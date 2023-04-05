#include "../include/pageLib.h"
#include "../include/tinyxml2/tinyxml2.h"
#include <sw/redis++/redis++.h>
#include <regex>
#include <iostream>
#include <fstream>
#include <string.h>
#include <dirent.h>

using std::ofstream;
using std::cout;
using std::endl;
using std::cerr;
using std::regex;
using std::to_string;
using namespace tinyxml2;
using namespace sw::redis;

struct RssItem
{
    string url;
    string title;
    string content;
};

string regexNode(const string &rhs)
{
	string pattern("<.*?>");
	regex r(pattern);
	string str = rhs;
	return regex_replace(str,r,"");
}

void PageLib::scanXml()
{
    DIR *dirp = opendir(_pwd.c_str());
    if(nullptr == dirp)
    {
        perror("opendir");
    }
    struct dirent *pdirent;
    while((pdirent = readdir(dirp)) != nullptr)
    {
        if(strcmp(pdirent->d_name,".") == 0 || strcmp(pdirent->d_name,"..") == 0)
        {
            continue;
        }
        string pwd = _pwd + pdirent->d_name;
        _dirScanner.push_back(pwd);
    }

    closedir(dirp);
}

PageLib::PageLib(string pwd)
: _pwd(pwd)
, _files(new vector<pair<string,int>>)
{
}

void PageLib::create()
{
    int docid = 1;
    // int flag = 0;
    for(auto it = _dirScanner.begin(); it != _dirScanner.end(); ++it){
        XMLDocument doc;
		if(doc.LoadFile(it->c_str()) != 0)
		{
			cout << "load xml file failed" << endl;
			return;
		}
		XMLElement *root = doc.RootElement();
		XMLElement *channel = root->FirstChildElement("channel");

		XMLElement *item = channel->FirstChildElement("item");
		while(item != nullptr)
		{
			XMLElement *title = item->FirstChildElement("title");
			XMLElement *url= item->FirstChildElement("link");
			XMLElement *content = item->FirstChildElement("description");
			RssItem node;
			node.title = title->GetText();
			node.url = url->GetText();
            if(content != nullptr)
            {
                node.content = regexNode(content->GetText());
            }

            string fmtTxt = "<doc><docid>" + to_string(docid) +
                            "</docid><url>" + node.url +
                            "</url><title>" + node.title +
                            "</title><content>" + node.content +
                            "</content></doc>";
            
            _files->push_back(std::make_pair(fmtTxt,0));
            // pair<int,int> pa = {flag,fmtTxt.size()};
            // pair<int,pair<int,int>> pa1 = {docid,pa};
            // _officeLib.insert(pa1);

            // flag += fmtTxt.size();
            ++docid;

			item = item->NextSiblingElement();
		}
    }
}

void PageLib::store()
{
    ofstream ofs("../data/ripepage.dat");
    if(!ofs.good()){
        cerr << "ofstream open file faild!" << endl;
        return;
    }
    auto redis = Redis("tcp://127.0.0.1:6379/4");
    redis.flushdb();

    for(int i = 0; i < (*_files).size(); ++i)
    {
        if(0 == (*_files)[i].second)
        {
            ofs << (*_files)[i].first;
            redis.set(std::to_string(i+1),(*_files)[i].first);
        }
    }
    ofs.close();

    ofstream ofs2("../data/offset.dat");
    int flag = 0;
    int i = 1;
    for(auto it = _files->begin(); it != _files->end(); ++it)
    {
        if(0 == it->second)
        {
            _officeLib.insert(std::make_pair(i,std::make_pair(flag,it->first.size())));       
            flag += it->first.size();
        }
        ++i;
    }
    for(auto &elem : _officeLib)
    {
        ofs2 << elem.first << " " << elem.second.first << " " << elem.second.second << endl;
    }
    ofs2.close();
}

shared_ptr<vector<pair<string,int>>> &PageLib::getFiles()
{
    return _files;
}

