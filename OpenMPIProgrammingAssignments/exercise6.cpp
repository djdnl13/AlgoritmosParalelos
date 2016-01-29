#include <iostream>
#include <fstream>
#include <string.h>
#include <queue>
#include <omp.h>

using namespace std;

const int MAX_LINES = 10000;
const int MAX_LINE = 1000;

int main(int argc, char* argv[]) {
	int thread_count;
	char* lines[MAX_LINES];
	int line_count;	
	queue<string> shared_queue;
	
	int j,i = 0;
	thread_count = strtol(argv[1], NULL, 10);


   #pragma omp parallel num_threads(thread_count) \
	default(none) \
	shared(shared_queue, thread_count, i)
	for(i=0 ; i<thread_count ; ++i)
	{
		if(!(i&1))
		{
			ifstream myfile("myfile"+to_string(i/2)+".txt", ifstream::in);
			char* line = new char[MAX_LINE];
			myfile.getline(line, MAX_LINE);
			while (!myfile.eof())
			{
				shared_queue.push(line);				
				myfile.getline(line, MAX_LINE);
			}
		}
		else
		{	
			while(!shared_queue.empty())				
			{
				char *my_token, *saveptr;
				string line;
				line = shared_queue.front();
				shared_queue.pop();
				printf("%s ", line);
				char *cstr = new char[line.length()];
				strcpy(cstr, line.c_str());

				my_token = strtok_r(cstr, " \t\n", &saveptr);
				while ( my_token != NULL )
				{
					printf("%s, ", &my_token);
					my_token = strtok_r(NULL, " \t\n", &saveptr);
				} 
				printf("\n");
			}
		}
	}
	return 0;
}
