#include "HttpSession.h"
#include "HttpMethod.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <stdio.h>

void HttpMethodGet(const HttpRequestContext &httprequestcontext, std::string &responsecontext, std::string path)
{
	//std::cout<<"path="<<path<<std::endl;
	path = "." + UrlDecode(path);
	if (path == "./" || path == "." || path == "./index.html")
		path = "./index.html";
	int split_pos = path.find("?");
	//std::cout << "Now is GET " << std::endl;

	if (split_pos != std::string::npos) 
		execCGI(path.substr(split_pos+1), responsecontext, path.substr(0, split_pos));
	else if (path.find_last_of(".php") != std::string::npos) {
		//std::cout<<"path=1"<<std::endl;
		execCGI(path, responsecontext, "");
	} else
	{
		//std::cout<<"path=12"<<std::endl;
		readFile(path, responsecontext);
	}
	return;
}

void HttpMethodPost(const HttpRequestContext &httprequestcontext, std::string &responsecontext, std::string path)
{
	//std::cout << "Now is POST" << std::endl;
	path = "." + UrlDecode(path);

	if(path.find_last_of(".php") != std::string::npos) 
		execCGI(UrlDecode(httprequestcontext.body), responsecontext, path);
	else{
		std::cout << "目前仅支持php" << std::endl;
	};
	return;
}

void readFile(std::string path, std::string & responsecontext)
{
	std::cout << "ReadFile" << std::endl;
	FILE *ptr = fopen(path.data(), "r");
	char buffer[1025];
	std::string responsebody = "";

	std::fill(buffer, buffer+sizeof(buffer), 0);
	if (ptr != NULL) {
		while (fgets(buffer, 1024, ptr) != NULL)  {
			responsebody.append(buffer);
		}
		fclose(ptr);
		ptr = NULL;
	} else {
		//std::cout << "NULL" << std::endl;
	}
	std::cout << "readFile : responsebody" << responsebody << std::endl;
	responsecontext +=  "HTTP/1.0 200 OK\r\n";
        responsecontext += "Server: Zhu ShuoCheng's NetServer/0.1\r\n";
        responsecontext += "Content-Type: text/html \r\n";
        responsecontext += "Content-Length: " + std::to_string(responsebody.size()) + "\r\n";
        responsecontext += "\r\n";
	responsecontext += responsebody;
}

void execCGI(std::string args, std::string &responsecontext, std::string path)
{
	std::string cmd = "/usr/bin/php " + path + " \"" + args +"\" 2>>/dev/null";
	FILE *ptr = popen(cmd.data(), "r");
	//std::cout << "path=" <<path<< std::endl;
	char buffer[1025];
	std::string responsebody = "";
	//std::cout << "args=" <<args<< std::endl;
	std::fill(buffer, buffer+sizeof(buffer), 0);
	if (ptr !=NULL&&args!="./cs") {
		//std::cout<<"zsc"<<std::endl;
		while (fgets(buffer, 1024, ptr) != NULL)  {
			responsebody.append(buffer);
		}
		pclose(ptr);
		ptr = NULL;
	} else {
		
		responsebody += "<html><title>出错了</title>";
		responsebody += "<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>";
		responsebody += "<style>body{background-color:#f;font-size:14px;}h1{font-size:60px;color:#eeetext-align:center;padding-top:30px;font-weight:normal;}</style>";
		responsebody += "<body bgcolor=\"ffffff\"><h1>";
		responsebody += std::to_string(404) + " " + "Not Found";
		responsebody += "</h1><hr><em> Zhu shuocheng's NetServer</em>\n</body></html>";

		std::string httpversion;
		httpversion = "HTTP/1.1";
	
			
		responsecontext += httpversion + " " + std::to_string(404) + " " + "Not Found" + "\r\n";
		responsecontext += "Server: Zhu shuocheng's NetServer/0.1\r\n";
		responsecontext += "Content-Type: text/html\r\n";
		responsecontext += "Connection: Keep-Alive\r\n";
		responsecontext += "Content-Length: " + std::to_string(responsebody.size()) + "\r\n";
		responsecontext += "\r\n";
		responsecontext += responsebody;
		return;
	}
	responsecontext +=  "HTTP/1.1 200 OK\r\n";
    responsecontext += "Server: Zhu ShuoCheng's NetServer/0.1\r\n";
    responsecontext += "Content-Type: text/html \r\n";
    responsecontext += "Content-Length: " + std::to_string(responsebody.size()) + "\r\n";
    responsecontext += "\r\n";
    responsecontext += responsebody;
}

std::string UrlDecode(std::string SRC) {
    std::string ret;
    char ch;
    int i, ii;
    for (i=0; i<SRC.length(); i++) {
        if (int(SRC[i])==37) {
            sscanf(SRC.substr(i+1,2).c_str(), "%x", &ii);
            ch=static_cast<char>(ii);
            ret+=ch;
            i=i+2;
        } else {
            ret+=SRC[i];
        }
    }
	//std::cout<<"ret="<<ret<<std::endl;
    return (ret);
}
