char* getfilenamefromurl(char* url){
	char* filename = NULL;
	const size_t urllen = strlen(url);
	for(int i=urllen-1;i>=0;i--){
		if(url[i]=='/'){
			filename = (char*)malloc(urllen-i);
			strcpy(filename, &url[i+1]);
			break;
		}
	}
	return filename;
}
