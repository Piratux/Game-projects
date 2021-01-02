#include "Logs.h"

void log_message(const char* msg)
{
	FILE* fp = fopen("GameData\\logs.txt", "a");
	fprintf(fp, "%s", msg);
	fclose(fp);
}
void log_program_start()
{
	time_t rawtime;
	struct tm* timeinfo;
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	log_message("Program started at:\n");
	log_message(asctime(timeinfo));
}
void log_program_end()
{
	time_t rawtime;
	struct tm* timeinfo;
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	log_message("Program ended at:\n");
	log_message(asctime(timeinfo));
	log_message("-------------------\n");
}
