// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
// cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"
#include "header/std_lib.h"
#include "header/filesystem.h"
#include "header/shell.h"

int main()
{
    char buf[128];
    fillKernelMap();
    makeInterrupt21();
    clearScreen();

    printString("             .___________. __    __  .______     ___        _______.\n");
    printString("             |           ||  |  |  | |   _  \\   / _ \\      /       |\n");
    printString("             `---|  |----`|  |  |  | |  |_)  | | | | |    |   (----`\n");
    printString("                 |  |     |  |  |  | |   _  <  | | | |     \\   \\    \n");
    printString("                 |  |     |  `--'  | |  |_)  | | |_| | .----)   |   \n");
    printString("                 |__|      \\______/  |______/   \\___/  |_______/    \n");
    printString("                                    Halo dunia!\r\n");

    while (1)
    {
        shell();
    }
}

// TODO
void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    switch (AX)
    {
    case 0x0:
        printString(BX);
        break;
    case 0x1:
        readString(BX);
        break;
    case 0x2:
        readSector(BX, CX);
        break;
    case 0x3:
        writeSector(BX, CX);
        break;
    case 0x4:
        read(BX, CX);
        break;
    case 0x5:
        write(BX, CX);
        break;
    case 0x6:
        executeProgram(BX, CX);
        break;
    default:
        printString("Invalid interrupt");
    }
}

void fillKernelMap()
{
    struct map_filesystem map_fs_buffer;
    int i;

    // Load filesystem map
    readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);

    // Edit filesystem map disini
    for (i = 0; i < 16; i++)
    {
        map_fs_buffer.is_filled[i] = true;
    }
    for (i = 256; i < 512; i++)
    {
        map_fs_buffer.is_filled[i] = true;
    }

    // Update filesystem map
    writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
}

void printString(char *string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        interrupt(0x10, 0x0E00 + string[i], 0, 0, 0);
        if (string[i] == '\n')
        {
            interrupt(0x10, 0x0E00 + 0xd, 0, 0, 0);
        }
        i++;
    }
}

void readString(char *string)
{
    char ch;
    int idx = 0;

    int flag = 1;
    while (flag)
    {
        ch = interrupt(0x16, 0, 0, 0, 0);
        if (ch == '\r' || ch == '\n')
        {
            string[idx] = '\0';
            interrupt(0x10, 0x0E00 + '\n', 0, 0, 0); // LF
            interrupt(0x10, 0x0E00 + '\r', 0, 0, 0); // CR
            flag = 0;
        }
        else if (ch == '\b')
        {
            if (idx > 0 && mod(idx, 80) != 0)
            {
                interrupt(0x10, 0x0E00 + '\b', 0, 0, 0); // backspace
                interrupt(0x10, 0x0E00 + '\0', 0, 0, 0); // terminator
                interrupt(0x10, 0x0E00 + '\b', 0, 0, 0); // backspace
                idx--;
            }
        }
        else
        {
            string[idx] = ch;
            interrupt(0x10, 0x0E00 + ch, 0, 0, 0);
            idx++;
        }
    }
}

void clearScreen()
{
    interrupt(0x10, 0x3, 0, 0, 0);                          // Ubah video mode menjadi 3
    interrupt(0x10, (6 << 8), 0xF << 8, 0, (24 << 8) + 79); // Hapus layar dengan scroll up window & ubah warna buffer
    interrupt(0x10, 0x200, 0, 0, 0);                        // Set cursor ke pojok kiri atas
}

void printCWD(char *path, byte cwd)
{
    struct node_filesystem node_fs_buffer;
    byte path[9];
    byte dir = cwd;
    int len = 0;

    readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    while (dir != FS_NODE_P_IDX_ROOT)
    {
        path[len] = dir;
        len++;
        dir = node_fs_buffer.nodes[dir].parent_node_index;
    }

    while (len > 0)
    {
        printString("/");
        printString(node_fs_buffer.nodes[path[len - 1]].name);
        len--;
    }
    printString("/");
}

void writeSector(byte *buffer, int sector_number)
{
    int sector_write_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector = mod(sector_number, 18) + 1;    // CL

    head = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00;                               // DL

    interrupt(
        0x13,                        // Interrupt number
        0x0300 | sector_write_count, // AX
        buffer,                      // BX
        cylinder | sector,           // CX
        head | drive                 // DX
    );
}
void readSector(byte *buffer, int sector_number)
{
    int sector_read_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector = mod(sector_number, 18) + 1;    // CL

    head = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00;                               // DL

    interrupt(
        0x13,                       // Interrupt number
        0x0200 | sector_read_count, // AX
        buffer,                     // BX
        cylinder | sector,          // CX
        head | drive                // DX
    );
}

void write(struct file_metadata *metadata, enum fs_retcode *return_code)
{
    // Tambahkan tipe data yang dibutuhkan
    struct map_filesystem map_fs_buffer;
    struct node_filesystem node_fs_buffer;
    struct sector_filesystem sector_fs_buffer;
    struct sector_entry sector_entry_buffer;
    char c[2];
    int idx_node;
    int idx_sector;
    bool empty_node = false;
    bool empty_sector = false;
    int total_empty_map = 0;
    int empty_space = 0;
    int i;
    int j = 0;

    // Masukkan filesystem dari storage ke memori
    readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
    readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);
    readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);

    // 1. Cari node dengan nama dan lokasi parent yang sama pada node.
    //    Jika tidak ditemukan kecocokan, lakukan proses ke-2.
    //    Jika ditemukan node yang cocok, tuliskan retcode
    //    FS_W_FILE_ALREADY_EXIST dan keluar.
    for (i = 0; i < 64; i++)
    {
        if (node_fs_buffer.nodes[i].parent_node_index == metadata->parent_index && strcmp(node_fs_buffer.nodes[i].name, metadata->node_name))
        {
            printString("File already exist");
            *return_code = FS_W_FILE_ALREADY_EXIST;
            return;
        }
    }

    // 2. Cari entri kosong pada filesystem node dan simpan indeks.
    //    Jika ada entry kosong, simpan indeks untuk penulisan.
    //    Jika tidak ada entry kosong, tuliskan FS_W_MAXIMUM_NODE_ENTRY
    //    dan keluar.
    for (i = 0; i < 64; i++)
    {
        if (node_fs_buffer.nodes[i].name[0] == '\0')
        {
            idx_node = i;
            empty_node = true;
            break;
        }
    }
    if (!empty_node)
    {
        printString("Maximum node entry");
        *return_code = FS_W_MAXIMUM_NODE_ENTRY;
        return;
    }

    // 3. Cek dan pastikan entry node pada indeks P adalah folder.
    //    Jika pada indeks tersebut adalah file atau entri kosong,
    //    Tuliskan retcode FS_W_INVALID_FOLDER dan keluar.
    if (metadata->parent_index != FS_NODE_P_IDX_ROOT &&
        !(node_fs_buffer.nodes[metadata->parent_index].sector_entry_index == FS_NODE_S_IDX_FOLDER))
    {
        printString("Invalid folder");
        *return_code = FS_W_INVALID_FOLDER;
        return;
    }

    // 4. Dengan informasi metadata filesize, hitung sektor-sektor
    //    yang masih kosong pada filesystem map. Setiap byte map mewakili
    //    satu sektor sehingga setiap byte mewakili 512 bytes pada storage.
    //    Jika empty space tidak memenuhi, tuliskan retcode
    //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
    //    Jika ukuran filesize melebihi 8192 bytes, tuliskan retcode
    //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
    //    Jika tersedia empty space, lanjutkan langkah ke-5.
    for (i = 0; i < 256; i++)
    {
        if (!map_fs_buffer.is_filled[i])
        {
            total_empty_map++;
        }
    }

    empty_space = total_empty_map * 512;

    if (empty_space < metadata->filesize || metadata->filesize > 8192)
    {
        printString("Not enough storage");
        *return_code = FS_W_NOT_ENOUGH_STORAGE;
        return;
    }

    // 5. Cek pada filesystem sector apakah terdapat entry yang masih kosong.
    //    Jika ada entry kosong dan akan menulis file, simpan indeks untuk
    //    penulisan.
    //    Jika tidak ada entry kosong dan akan menulis file, tuliskan
    //    FS_W_MAXIMUM_SECTOR_ENTRY dan keluar.
    //    Selain kondisi diatas, lanjutkan ke proses penulisan.
    for (i = 0; i < 32; i++)
    {
        if (sector_fs_buffer.sector_list[i].sector_numbers[0] == 0)
        {
            idx_sector = sector_fs_buffer.sector_list[i].sector_numbers;
            empty_sector = true;
            break;
        }
    }
    if (!empty_sector)
    {
        printString("Maximum sector entry");
        *return_code = FS_W_MAXIMUM_SECTOR_ENTRY;
        return;
    }

    // Penulisan
    // 1. Tuliskan metadata nama dan byte P ke node pada memori buffer
    strcpy(node_fs_buffer.nodes[idx_node].name, metadata->node_name);
    node_fs_buffer.nodes[idx_node].parent_node_index = metadata->parent_index;
    // 2. Jika menulis folder, tuliskan byte S dengan nilai
    //    FS_NODE_S_IDX_FOLDER dan lompat ke langkah ke-8
    // 3. Jika menulis file, tuliskan juga byte S sesuai indeks sector
    if (metadata->filesize == 0)
    {
        node_fs_buffer.nodes[idx_node].sector_entry_index = FS_NODE_S_IDX_FOLDER;
    }
    else
    {
        node_fs_buffer.nodes[idx_node].sector_entry_index = idx_sector;
    }
    // 4. Persiapkan variabel j = 0 untuk iterator entry sector yang kosong
    // 5. Persiapkan variabel buffer untuk entry sector kosong
    // 6. Lakukan iterasi berikut dengan kondisi perulangan (penulisan belum selesai && i = 0..255)
    //    1. Cek apakah map[i] telah terisi atau tidak
    //    2. Jika terisi, lanjutkan ke iterasi selanjutnya / continue
    //    3. Tandai map[i] terisi
    //    4. Ubah byte ke-j buffer entri sector dengan i
    //    5. Tambah nilai j dengan 1
    //    6. Lakukan writeSector() dengan file pointer buffer pada metadata
    //       dan sektor tujuan i
    //    7. Jika ukuran file yang telah tertulis lebih besar atau sama dengan
    //       filesize pada metadata, penulisan selesai
    for (i = 0; i < 255; i++)
    {
        if (!map_fs_buffer.is_filled[i])
        {
            map_fs_buffer.is_filled[i] = true;
            sector_entry_buffer.sector_numbers[j] = i;
            j++;
            writeSector(metadata->buffer, i);
            metadata->filesize -= 512;
            metadata->buffer += 512;
            if (metadata->filesize <= 0)
            {
                break;
            }
        }
    }

    // 7. Lakukan update dengan memcpy() buffer entri sector dengan
    //    buffer filesystem sector
    memcpy(sector_fs_buffer.sector_list[idx_sector].sector_numbers, sector_entry_buffer.sector_numbers, 16);
    // 8. Lakukan penulisan seluruh filesystem (map, node, sector) ke storage
    //    menggunakan writeSector() pada sektor yang sesuai
    writeSector(&map_fs_buffer.is_filled, FS_MAP_SECTOR_NUMBER);
    writeSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
    writeSector(&sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);
    // 9. Kembalikan retcode FS_SUCCESS
    *return_code = FS_SUCCESS;
}

void read(struct file_metadata *metadata, enum fs_retcode *return_code)
{
    // Tambahkan tipe data yang dibutuhkan
    struct node_filesystem node_fs_buffer;
    struct node_entry node_entry_buffer;
    struct sector_filesystem sector_fs_buffer;
    struct sector_entry sector_entry_buffer;
    int i;
    byte idx_sector = -1;

    // Masukkan filesystem dari storage ke memori buffer
    readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
    readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);

    // 1. Cari node dengan nama dan lokasi yang sama pada filesystem.
    //    Jika ditemukan node yang cocok, lanjutkan ke langkah ke-2.
    //    Jika tidak ditemukan kecocokan, tuliskan retcode FS_R_NODE_NOT_FOUND
    //    dan keluar.
    for (i = 0; i < 64; i++)
    {
        memcpy(&node_entry_buffer, &(node_fs_buffer.nodes[i]), sizeof(struct node_entry));

        if (node_entry_buffer.parent_node_index == metadata->parent_index && strcmp(node_entry_buffer.name, metadata->node_name))
        {
            idx_sector = node_entry_buffer.sector_entry_index;
            break;
        }
    }

    if (idx_sector == -1)
    {
        *return_code = FS_R_NODE_NOT_FOUND;
        return;
    }

    // 2. Cek tipe node yang ditemukan
    //    Jika tipe node adalah file, lakukan proses pembacaan.
    //    Jika tipe node adalah folder, tuliskan retcode FS_R_TYPE_IS_FOLDER
    //    dan keluar.
    if (idx_sector == FS_NODE_S_IDX_FOLDER)
    {
        *return_code = FS_R_TYPE_IS_FOLDER;
        return;
    }
    // Pembacaan
    // 1. memcpy() entry sector sesuai dengan byte S
    // 2. Lakukan iterasi proses berikut, i = 0..15
    // 3. Baca byte entry sector untuk mendapatkan sector number partisi file
    // 4. Jika byte bernilai 0, selesaikan iterasi
    // 5. Jika byte valid, lakukan readSector()
    //    dan masukkan kedalam buffer yang disediakan pada metadata
    // 6. Lompat ke iterasi selanjutnya hingga iterasi selesai
    // 7. Tulis retcode FS_SUCCESS dan ganti filesize pada akhir proses pembacaan.
    memcpy(sector_entry_buffer.sector_numbers, &(sector_fs_buffer.sector_list[idx_sector]), sizeof(struct sector_entry));

    for (i = 0; i < 16; i++)
    {
        idx_sector = sector_entry_buffer.sector_numbers[i];
        if (idx_sector == 0)
        {
            break;
        }
        else
        {
            readSector(&(metadata->buffer[i * 512]), idx_sector);
        }
        metadata->filesize = 512 * i;
    }
    *return_code = FS_SUCCESS;
}

void shell()
{
    int i;
    int j;
    int paramLen;
    char param[8][64];
    char input_buf[64];
    char path_str[128];
    byte current_dir = FS_NODE_P_IDX_ROOT;

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 64; j++)
        {
            param[i][j] = 0;
        }
    }
    while (true)
    {
        printString("Tub0S@IF2230:");
        printCWD(path_str, current_dir);
        printString("$");
        readString(input_buf);
        paramLen = splitParam(input_buf, &param);
        if (strcmp(param[0], "cd"))
        {
            // Utility cd
            cd(current_dir, param[1], &current_dir);
        }
        else if (strcmp(param[0], "ls"))
        {
            // util ls
            ls(param[1], current_dir);
        }
        else if (strcmp(param[0], "mv"))
        {
            // util mv
            mv(current_dir, param[1], param[2]);
        }
        else if (strcmp(param[0], "mkdir"))
        {
            // util mkdir
            mkdir(param[1], current_dir);
        }
        else if (strcmp(param[0], "cat"))
        {
            // util cat
            cat(param[1], current_dir);
        }
        else if (strcmp(param[0], "cp"))
        {
            // util cp
            cp(param[1], current_dir, param[2]);
        }
        else
            printString("Unknown command\r\n");
        clear(input_buf, 64);
        clear(param, 512);
    }
}

// Returns number of parameters
int splitParam(char *input, char param[8][64])
{
    int i = 0;
    int j = 0;
    int paramLen = 0;
    int inputLen = strlen(input);
    while (i < inputLen)
    {
        j = 0;
        while (input[i] != ' ' && input[i] != '\0')
        {
            param[paramLen][j] = input[i];
            i++;
            j++;
        }
        j++;
        i++;
        param[paramLen][j] = '\0';
        paramLen++;
    }
    return paramLen;
}

void executeProgram(struct file_metadata *metadata, int segment)
{
    enum fs_retcode fs_ret;
    byte buf[8192];
    metadata->buffer = buf;
    read(metadata, &fs_ret);
    if (fs_ret == FS_SUCCESS)
    {
        int i = 0;
        for (i = 0; i < 8192; i++)
        {
            if (i < metadata->filesize)
                putInMemory(segment, i, metadata->buffer[i]);
            else
                putInMemory(segment, i, 0x00);
        }
        launchProgram(segment);
    }
    else
        printString("exec: file not found\r\n");
}