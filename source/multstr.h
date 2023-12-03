char* multstr(const char str[],const int mult){
	const int len=strlen(str);
	char* out = malloc(len*mult+1);
	for(int i=0;i<mult;i++){
		strcpy(&out[i*len],str);
	}
	out[len*mult]='\0';
	return out;
}
