#define NSEC 1000000000

double tvcalc_time(struct timespec start,struct timespec stop){
	return (double)(stop.tv_sec - start.tv_sec) +(double)(stop.tv_nsec - start.tv_nsec)/NSEC;
}
