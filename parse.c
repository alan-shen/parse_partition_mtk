#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define DEBUG (0)
#define DBG if(DEBUG)printf

#define PROCINFO_PART   "info/partinfo"
#define OTAINFO_SCATTER "info/scatter"

struct partition_info{
	char p_name[32];
	char p_addr[32];
	char p_size[32];
};

struct partition_info procpartinfo[30];
struct partition_info scatterinfo[30];

int parse_procpartinfo(void)
{
	FILE *fp_info;
	char buf[512];
	int  pNumProc = 0;

	fp_info = fopen(PROCINFO_PART, "r");
	if (fp_info) {
		DBG("\nProc Partinfo Partition Information:\n\n");
		while (fgets(buf, sizeof(buf), fp_info)) {
			if (sscanf(buf, "%s %s %s", procpartinfo[pNumProc].p_name, procpartinfo[pNumProc].p_addr, procpartinfo[pNumProc].p_size) == 3) { 
				DBG("[%02d] %s %s %s\n", pNumProc, procpartinfo[pNumProc].p_name, procpartinfo[pNumProc].p_addr, procpartinfo[pNumProc].p_size);
			}
			pNumProc++;
		}
		fclose(fp_info);
	}

	return pNumProc;
}

int parse_otascatter(void)
{
	FILE *fp_info;
	char buf[512];
	int  pNumScatter = 0;

	fp_info = fopen(OTAINFO_SCATTER, "r");
	if (fp_info) {
		DBG("\nScatter Partition Information:\n");
		while (fgets(buf, sizeof(buf), fp_info)) {
			if (sscanf(buf, "%s %s", scatterinfo[pNumScatter].p_name, scatterinfo[pNumScatter].p_addr) == 2) {
				if(!strcmp(scatterinfo[pNumScatter].p_name, "preloader")){
					continue;
				}
				DBG("[%02d] %s %s\n", pNumScatter, scatterinfo[pNumScatter].p_name, scatterinfo[pNumScatter].p_addr);
			}
			pNumScatter++;
		}
		fclose(fp_info);
	}

	return pNumScatter;
}

int show_partinfo(int pNumproc, int pNumota)
{
	int num;

	if(pNumproc != pNumota){
		return 1;
	}
	else{
		for(num=0; num<pNumproc; num++){
			if(!strcmp(procpartinfo[num].p_name, scatterinfo[num].p_name)){
				if(!strcmp(scatterinfo[num].p_name, "pgpt")){
					continue;
				}
				else if(!strcmp(scatterinfo[num].p_name, "flashinfo")){
					continue;
				}
				else if(!strcmp(scatterinfo[num].p_name, "sgpt")){
					continue;
				}
				else{
					printf("[%02d] %-12s : %s %s\n", num, procpartinfo[num].p_name, procpartinfo[num].p_addr, scatterinfo[num].p_addr);
				}
			}
			else{
				return 1;
			}
		}
	}

	return 0;
}

int main(int argc, char **args)
{
	int pNum_1 = 0;
	int pNum_2 = 0;
	int ret_compare = 0;

	printf("PARTITION:\n\n");

	pNum_1 = parse_procpartinfo();
	pNum_2 = parse_otascatter();

	ret_compare = show_partinfo(pNum_1, pNum_2);
	if(ret_compare == 0){
		printf("\n\t##### Same partition in Target and OTA #####\n\n");
	}
	else{
		printf("\n\t##### Different partition in Target and OTA, Not allow to update #####\n\n");
	}

    return 0;
}
