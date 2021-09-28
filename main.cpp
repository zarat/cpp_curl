#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;

static void print_cookies(CURL *curl)
{
    CURLcode res;
    struct curl_slist *cookies;
    struct curl_slist *nc;
    int i;
   
    printf("Cookies:\n");
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
    if(res != CURLE_OK) {
        fprintf(stderr, "Curl curl_easy_getinfo failed: %s\n", curl_easy_strerror(res));
        return;
    }
    nc = cookies;
    i = 1;
    while(nc) {
        printf("[%d]: %s\n", i, nc->data);
        nc = nc->next;
        i++;
    }
    if(i == 1) {
        printf("(none)\n");
    }
    curl_slist_free_all(cookies);
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
 
int main(int argc, char **argv)
{
                   
    CURL *curl;
    CURLcode res;
    
    char url1[] = "https://yt5s.com/api/ajaxSearch";
    char url2[] = "https://vp151.zzcdn.xyz/api/json/convert";
    
    FILE *fp;
        
    char videoLink[50] = {0};
    if(argc < 2) {
        scanf("%s", &videoLink);
    }    
    else
        strcat(videoLink, argv[1]);
                        
    std::string outfilename = "";
    std::string tmpfilename = "";
    std::string fileURL = "";
   
    if(1) {
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
 
    curl = curl_easy_init();
    
    if(curl) {
    
        curl_easy_setopt(curl, CURLOPT_URL, url1);
        
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        
        std::string postfields = "q=";
        postfields.append(videoLink);
        postfields.append("&vt=home");
        
        char *tok;
        tok = strtok(videoLink, "=");
        int itok = 0;
        while(NULL != tok) {
            if(itok == 1)
                tmpfilename.append(tok);
            tok = strtok(NULL, "=");
            itok++;
        }
        tmpfilename.append(".tmp");
        
        curl_easy_setopt(curl, CURLOPT_REFERER, "https://yt5s.com/en9");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields.c_str());    
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "mini crawler"); 
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
        //curl_easy_setopt(curl, CURLOPT_COOKIE, "name=xxx; name2=xxx;");        
        
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        fp = fopen(tmpfilename.c_str(),"wb");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        
        res = curl_easy_perform(curl);
    
        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
     
        fclose(fp);
        
        //print_cookies(curl);

        curl_easy_cleanup(curl);
      
    }
   
    curl_global_cleanup();
    
    }
    
    FILE* tf;
    tf = fopen(tmpfilename.c_str(), "r");
    fseek(tf, 0L, SEEK_END); 
    int len = ftell(tf); 
    rewind(tf);
    char c, buffer[len];    
    int i = 0;
    while( fread(&c, sizeof(char), 1, tf) > 0 ) 
        buffer[i++] = c;
    fclose(tf);
    buffer[i] = '\0';
  
    std::string str;
    str.append(buffer);
    
    //printf(buffer);
    
    nlohmann::json j = nlohmann::json::parse(str);
    
    std::string postfields;
    
    if(j.contains("vid")) {
        postfields.append("v_id=");
        postfields.append(j["vid"].get<std::string>());
    }
    
    postfields.append("&ftype=mp4&fquality=720p");
    
    if(j.contains("fn")) {
        postfields.append("&fname=");
        postfields.append(j["fn"].get<std::string>());
        
        //outfilename.append(j["fn"].get<std::string>());
        
        std::string str = j["fn"].get<std::string>();
        
        static string safeChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890");
        string strResult;
        for (std::string::size_type i = 0; i < str.size(); i++) {
            if (safeChars.find(str[i]) != std::string::npos)
                strResult.push_back(str[i]);
            else {
                strResult.append(" ");
            }
        }
    
        outfilename.append(strResult);
        outfilename.append(".mp4");
        
        //cout << "outfilename = " << outfilename;
        
    }
    
    if(j.contains("token")) {
        postfields.append("&token=");
        postfields.append(j["token"].get<std::string>());
    }
        
    if(j.contains("timeExpires")) {
        postfields.append("&timeExpire=");
        postfields.append(j["timeExpires"].get<std::string>());
    }
    
    
    //printf("%s", postfields.c_str());

    if(1) {
    
    // 2
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if(curl) {
    
        curl_easy_setopt(curl, CURLOPT_URL, url2);
        
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        
        curl_easy_setopt(curl, CURLOPT_REFERER, "https://yt5s.com/en9");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields.c_str());    
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "mini crawler"); 
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
        //curl_easy_setopt(curl, CURLOPT_COOKIE, "name=xxx; name2=xxx;");        
        
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        fp = fopen(tmpfilename.c_str(),"wb");
        
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
     
        fclose(fp);
        
        //print_cookies(curl);
     
        /* always cleanup */
        curl_easy_cleanup(curl);
      
    }
    
    tf = fopen(tmpfilename.c_str(), "r");
    fseek(tf, 0L, SEEK_END); 
    int len = ftell(tf); 
    rewind(tf);
    char c, buffer[len];    
    int i = 0;
    while( fread(&c, sizeof(char), 1, tf) > 0 ) 
        buffer[i++] = c;
    fclose(tf);
    buffer[i] = '\0';
  
    std::string str;
    str.append(buffer);
    
    j = nlohmann::json::parse(str);
    
    fileURL.append(j["result"].get<std::string>());
    
    //printf("%s", fileURL.c_str());
   
    curl_global_cleanup();
    
    }
    
    if(1) {
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
 
    curl = curl_easy_init();
    
    if(curl) {
    
        curl_easy_setopt(curl, CURLOPT_URL, fileURL.c_str());
        
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        fp = fopen(outfilename.c_str(),"wb");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        
        res = curl_easy_perform(curl);
    
        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
     
        fclose(fp);
        
        //print_cookies(curl);

        curl_easy_cleanup(curl);
      
    }
   
    curl_global_cleanup();
    
    }
    
    unlink(tmpfilename.c_str());
   
    return 0;
  
}