const short bytessize = 1024;
const short rows = 5;
const char* anns[] = {"b","Kb","Mb","Gb","Tb"};

char* annotateBytes(unsigned long bytes){
	int index = 0;
	double endbytes = (double)bytes;
	while(endbytes>=bytessize&&index<rows){
		index++;
		endbytes/=bytessize;
	}
	const int outsize = 5;
	char* out = (char*)malloc(outsize+strlen(anns[index]));
	snprintf(out, outsize, "%f",endbytes)<0?abort():0;
	strcpy(&out[outsize-1],anns[index]);
	return out;
}
