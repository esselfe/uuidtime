// uuidtime is a Linux program to generate an UUID(like) of current time based
// on seconds and microseconds since epoch(1970).
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef WINDOWS_BUILD
#include <sys/time.h>
#include <getopt.h>
#else
#define snprintf sprintf_s
#endif

const char *uuidtime_version = "0.1.4";

#ifndef WINDOWS_BUILD
static const struct option long_options[] = {
	{"help", no_argument, NULL, 'h'},
	{"version", no_argument, NULL, 'V'},
	{NULL, 0, NULL, 0}
};
static const char *short_options = "hV";
#else
#include <Windows.h>
#include <stdint.h> // for portable: uint64_t

/*struct timeval {
	long tv_sec;  // seconds
	long tv_usec; // microseconds
};*/

// Windows-based implementation of the POSIX call, according to MS Copilot
int gettimeofday(struct timeval* tp, struct timezone* tzp) {
	// Note: some broken versions only have 8 trailing zeroes,
	// the correct epoch has 9 trailing zeroes
	static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

	SYSTEMTIME system_time;
	FILETIME file_time;
	uint64_t time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((uint64_t)file_time.dwLowDateTime);
	time += ((uint64_t)file_time.dwHighDateTime) << 32;

	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	return 0;
}
#endif

// Program entry point
int main (int argc, char **argv) {
#ifndef WINDOWS_BUILD
	int opt;
	while (1) {
		opt = getopt_long(argc, argv, short_options, long_options, NULL);
		if (opt == -1)
			break;
		switch (opt) {
		case 'h':
			printf("Usage: uuidtime { -h/--help | -V/--version }\n");
			exit(0);
		case 'V':
			printf("uuidtime %s\n", uuidtime_version);
			exit(0);
		}
	}
#endif

	struct timeval tv0;
	gettimeofday(&tv0, NULL);
	time_t t0 = (time_t)tv0.tv_sec;
	struct tm *tm0 = gmtime(&t0);
	if (tm0 == NULL) {
		perror("gmtime");
		return 1;
	}

	// 36 characters, including the null-terminator
	char buffer[36];
	// d94deb38-e066-11e9-9324-001b77c4af72
	// yyyymmdd-0000-ssss-ssuu-uuuurrrrrrrr
	srand((unsigned int)tv0.tv_usec/100);
	snprintf(buffer, 35, "%04x%02x%02x%04x%06x%06x%02x%02x%02x",
		(int)tm0->tm_year+1900, (int)tm0->tm_mon, (int)tm0->tm_mday, 0,
		(int)tv0.tv_sec, (int)tv0.tv_usec, rand()%255, rand()%255, rand()%255);
	
	char *c = buffer;
	unsigned int cnt = 0;
	// Add dashes afterward since they cut in the middle of number sets
	while (1) {
		fputc(*c, stdout);
		if (cnt == 7 || cnt == 11 || cnt == 15 || cnt == 19)
			fputc('-', stdout);

		++cnt;
		++c;
		if (*c == '\0')
			break;
	}
	fputc('\n', stdout);
	fflush(stdout);

	return 0;
}

