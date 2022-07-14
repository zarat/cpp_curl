#include <iostream>
#include <curl/curl.h>

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(void)
{

  CURL *curl;
  CURLcode res;
  
  std::string readBuffer;

  curl = curl_easy_init();
  
  if(curl) {
  
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
    
    //curl_easy_setopt(curl, CURLOPT_REFERER, "https://zarat.local");
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");    
    //curl_easy_setopt(curl, CURLOPT_USERAGENT, "mini crawler"); 
    //curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
    //curl_easy_setopt(curl, CURLOPT_COOKIE, "name=xxx; name2=xxx;");                
    //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    std::cout << readBuffer << std::endl;
    
  }
  
  return 0;
  
}
