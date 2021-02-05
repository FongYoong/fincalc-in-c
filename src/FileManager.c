#include "FileManager.h"

char* fManager_ReadFile(FILE* file, bool closeStream) {
	char result[3000] = "\0", temp[3000] = "\0";
	//Store in temp[] and gradually append to result[].
	while (fgets(temp, 3000, file) != NULL) {
		strcat(result, temp);
	}
	if (closeStream) {
		fclose(file);
	}
	return result;
}
void fManager_WriteFile(FILE* file, char content[], bool closeStream) {
	fprintf(file, content);
	if (closeStream) {
		fclose(file);
	}
}