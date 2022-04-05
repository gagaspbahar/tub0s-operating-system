void cd(byte idxParent, char *param, byte *idxDir);

void ls(char *name, byte currDir);

void mv(byte currDir, char *name, char *target);

void mkdir(char *param, byte idxParent);

void cat(char *param, byte currDir);

void cp(char *param, byte currDir, char *target);