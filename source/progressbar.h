#define PERCENTAGE 100
const char arr[] = ",;";
const short arrlen = strlen(arr);

struct progress_bar{
	int maxhashes;
	int numhashes;
	char* progressbuff;
};

void init_progressbar(struct progress_bar* progressbar, int maxhashes){
	progressbar->maxhashes=maxhashes;
	progressbar->progressbuff=(char*)malloc(maxhashes+1);
	memset(progressbar->progressbuff, ' ', maxhashes);
	progressbar->progressbuff[maxhashes]='\0';
}

void update_progressbar(struct progress_bar* progressbar,float total,float part){
	const int maxhashes = progressbar->maxhashes;
	const int percentage = ((float)part/(float)total)*PERCENTAGE;
	const int percentagereqforhash = PERCENTAGE/maxhashes;
	progressbar->numhashes = percentage/percentagereqforhash;
	const int isit = (percentage/arrlen)%arrlen;
	for(int i=0;i<progressbar->numhashes;i++){
		progressbar->progressbuff[i]=i<progressbar->numhashes-1||progressbar->numhashes>=maxhashes?'#':arr[isit];
	}
}
