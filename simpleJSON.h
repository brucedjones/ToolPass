#ifndef SIMPLE_JSON_h
#define SIMPLE_JSON_h

class SimpleJSON {
public:
	static char * GetStrKey(char * key, int keyLen, char * buffer, int bufferLen);

	static int GetIntKey(char * key, int keyLen, char * buffer, int bufferLen);
};

#endif