#include "http.h"
#include <curl/curl.h>
#include "log.h"
size_t writeToString(void *ptr, size_t size, size_t count, void *stream)
{
	((std::string*)stream)->append((char*)ptr, 0, size* count);
	return size* count;
}
int Http::Get(const string& url, string& out)
{
	CURL *curl;
	CURLcode res;

	out.clear();
	curl = curl_easy_init();
	if (curl) {

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
		{
			LOG_ERROR << "curl_easy_perform() failed: " << curl_easy_strerror(res);
			return -1;
		}

		curl_easy_cleanup(curl);
		return 0;
	}
	return -1;
}

int Http::Post(const string& url,const string& postfields, string& out)
{
	CURL *curl;
	CURLcode res;

	out.clear();
	curl = curl_easy_init();
	if (curl) {

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields.c_str());    // 指定post内容 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);


		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
		{
			LOG_ERROR << "curl_easy_perform() failed: " << curl_easy_strerror(res);
			return -1;
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
		return 0;
	}
	return -1;
}