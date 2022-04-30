#include "../header/std_type.h"
#include "../header/shell.h"
#include "../header/filesystem.h"
#include "../header/program.h"
#include "../header/textio.h"
#include "../header/fileio.h"
#include "../header/utils.h"
#include "../header/string_lib.h"

int main()
{
    int i;
    int j;
    int paramLen;
    char param[8][64];
    char input_buf[64];
    char path_str[128];
    Message msg;
    byte current_dir = FS_NODE_P_IDX_ROOT;
    struct file_metadata meta;

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 64; j++)
        {
            param[i][j] = 0;
        }
    }
    while (true)
    {
        puts("Tub0S@IF2230:");
        printCWD(path_str, current_dir);
        puts("$");
        gets(input_buf);
        paramLen = splitParam(input_buf, &param);
        msg.current_directory = current_dir;
        memcpy(msg.arg1, param[0], 64);
        memcpy(msg.arg2, param[1], 64);
        memcpy(msg.arg3, param[2], 64);
        set_message(&msg);
        if (strcmp(param[0], "cd"))
        {
            // Utility cd
            meta.node_name = "cd";
            meta.parent_index = 0x0;
            exec(&meta, 0x3000);
        }
        else if (strcmp(param[0], "ls"))
        {
            // util ls
            meta.node_name = "ls";
            meta.parent_index = 0x0;
            exec(&meta, 0x3000);
        }
        else if (strcmp(param[0], "mv"))
        {
            // util mv
            meta.node_name = "mv";
            meta.parent_index = 0x0;
            exec(&meta, 0x3000);
        }
        else if (strcmp(param[0], "mkdir"))
        {
            // util mkdir
            meta.node_name = "mkdir";
            meta.parent_index = 0x0;
            exec(&meta, 0x3000);
        }
        else if (strcmp(param[0], "cat"))
        {
            // util cat
            meta.node_name = "cat";
            meta.parent_index = 0x0;
            exec(&meta, 0x3000);
        }
        else if (strcmp(param[0], "cp"))
        {
            // util cp
            meta.node_name = "cp";
            meta.parent_index = 0x0;
            exec(&meta, 0x3000);
        }
        else if (strcmp(param[0], "test"))
        {
            meta.node_name = "shell";
            meta.parent_index = 0x0;
            // interrupt(0x21, 0x6, &meta, 0x2000, 0x0);
            exec(&meta, 0x2000);
        }
        else{
            puts("Unknown command\r\n");
        }
        clear(input_buf, 64);
        clear(param, 512);
    }
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
        puts("/");
        puts(node_fs_buffer.nodes[path[len - 1]].name);
        len--;
    }
    puts("/");
}