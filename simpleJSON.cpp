#include <Arduino.h>

#include "simpleJSON.h"

char * SimpleJSON::GetStrKey(char * key, int keyLen, char * buffer, int bufferLen)
{
	bool keyFound = false;
	int i;
	for (i = 0; i < bufferLen; i++)
	{
		if(buffer[i] == key[0]){
			for (int j = 0; j < keyLen; ++j)
			{
				if(!(buffer[i+j] == key[j])) break;
				if(j==keyLen-1) keyFound = true;
			}
		}
		if(keyFound) break;
	}

	int quoteCount = 0;
	for(; i<bufferLen; i++)
	{
		if(buffer[i] == '"') quoteCount++;
		if(quoteCount == 2) return &buffer[i+1];
	}
}

int SimpleJSON::GetIntKey(char * key, int keyLen, char * buffer, int bufferLen)
{
	return 0;
}