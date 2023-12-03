#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>
#include "annotateBytes.h"
#include "multstr.h"
#include "progressbar.h"
#include "tvcalc_time.h"
#include "getfilenamefromurl.h"

static size_t
writecallback(char* contents, size_t size, size_t nmemb, FILE* stream){
	size_t datawritten = fwrite(contents,size,nmemb,stream);
	return datawritten;
}

struct progress_data{
	curl_off_t dlprev;
	struct timespec progress_start;
	struct timespec start;
	long int speed;
	struct progress_bar progressbar;
};

int dlinfocallback(void* userp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t uptotal, curl_off_t upnow){
	struct progress_data* progressdata = (struct progress_data*)userp;

	struct progress_bar* progressbar = &progressdata->progressbar;

	if(dltotal){
		update_progressbar(progressbar,dltotal,dlnow);
	}

	struct timespec stop;
	clock_gettime(CLOCK_MONOTONIC, &stop);
	const double seconds = tvcalc_time(progressdata->progress_start,stop);
	if(!((int)(seconds*100)%100)){
		double dlpart = dlnow - progressdata->dlprev;
		if(dlpart>0){
			double time = tvcalc_time(progressdata->start,stop);
			progressdata->speed = (long int)(dlpart/time);
			clock_gettime(CLOCK_MONOTONIC, &progressdata->start);
			progressdata->dlprev = dlnow;
		}
	}
	const char* dlnowstr=annotateBytes(dlnow);
	const char* dltotalstr=annotateBytes(dltotal);
	const int numhashes = progressbar->numhashes;
	const int spacing=3;
	printf("\033[2K%s%s%sTime: %.2lf\n%s[%s] Est Speed: %s/s\n%s^%sAvg Speed: %s/s\n%s%s\033[3F",
			multstr(" ",spacing+progressbar->maxhashes+2-strlen(dltotalstr)/2),dltotalstr,multstr(" ",5),seconds,
			multstr(" ",spacing),progressbar->progressbuff,annotateBytes(progressdata->speed),
			multstr(" ",numhashes+spacing),multstr(" ",progressbar->maxhashes-numhashes+2),annotateBytes(dlnow/seconds),
		    multstr(" ",numhashes+spacing-strlen(dlnowstr)/2),dlnowstr);
	free((void*)dlnowstr);
	free((void*)dltotalstr);
	fflush(stdout);
	return 0;
}

int main(int argc, char* args[]){ //https://jsoncompare.org/LearningContainer/SampleFiles/Video/MP4/Sample-MP4-Video-File-for-Testing.mp4
	struct progress_data progressdata;
	clock_gettime(CLOCK_MONOTONIC, &progressdata.progress_start);
	init_progressbar(&progressdata.progressbar,25);
	//args[1]="https://jsoncompare.org/LearningContainer/SampleFiles/Video/MP4/Sample-MP4-Video-File-for-Testing.mp4";
	CURL *curl;
	CURLcode res;
	char* filename = getfilenamefromurl(args[1]);
	FILE* file = fopen(filename,"wb");
	free(filename);

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, args[1]);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, dlinfocallback);
	curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &progressdata);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

	res = curl_easy_perform(curl);
	if(res!=CURLE_OK){
		printf("Failed performing actions\n%s", curl_easy_strerror(res));
	}else{

	}
	printf("%s",multstr("\n",4));
	curl_easy_cleanup(curl);

	fclose(file);
	curl_global_cleanup();

	return 0;
}
