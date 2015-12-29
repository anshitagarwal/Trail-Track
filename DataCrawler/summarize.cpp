#include<stdio.h>
#include<string.h>
#include<fstream>
#include<cstdlib>
#include<conio.h>
#include<iostream>
#include <unistd.h>

char stopwords[1500][25],tags[200][20],name[50],dataname[500],metadataname[500],reportname[500],linkedinname[500],tag[50],cwd[500];
int tagcount=0,stopcount=0;

void getTagsAndStopWords()
{
    char temp[20];
    char tagsfile[500];
    char stopwordsfile[500];
    strcpy(tagsfile,cwd);
    strcat(tagsfile,"\\DataCrawler\\tags.txt");
    strcpy(stopwordsfile,cwd);
    strcat(stopwordsfile,"\\DataCrawler\\stopwords.txt");

    std::ifstream file(tagsfile);
    std::ifstream stopwordfile(stopwordsfile);

    //get tags
    while(file.getline(temp,20,' '))
    {
        strcpy(tags[tagcount]," ");
        if(temp[0]=='\n')
            strcat(tags[tagcount++],temp+1);
        else
            strcat(tags[tagcount++],temp);
    }
    file.close();

    //get stop words
    while(stopwordfile.getline(temp,25,'\n'))
    {
        strcpy(stopwords[stopcount++],temp);
    }
    stopwordfile.close();
}

void crawlData()
{
    //char command[500]="python crawl.py ";
    char command[500]="C:\\Python35\\python ";
    strcat(command,cwd);
    strcat(command,"\\DataCrawler\\crawl.py ");
    int i;

    //printf("Enter name:");
    //gets(name);

    //printf("Enter extra tag:");
    //gets(tag);

    /*for(i=0;name[i]!='\0';i++)
    {
        if(name[i]==' ')
            name[i]='+';
    }
    for(i=0;tag[i]!='\0';i++)
    {
        if(tag[i]==' ')
            tag[i]='+';
    }*/

    strcat(command,name);
    strcat(command," ");
    strcat(command,tag);
    system(command);
}

void cleanData()
{
    char sentence[3000];
    char datanametemp[50];
    int i;

    strcpy(datanametemp,cwd);
    strcat(datanametemp,"\\");
    strcat(datanametemp,name);
    strcat(datanametemp,"\\");
    strcat(datanametemp,"temp.txt");

    std::ifstream file(dataname);
    std::ofstream outputfile(datanametemp);
    while(!file.eof())
    {

        file.getline(sentence,2999,'.');
        if(file.eof())
            break;
        strcat(sentence,".");
        for(i=0;i<sentence[i]!='\0';i++)
        {
            if(sentence[i]<0||sentence[i]>255)
                sentence[i]=' ';
            if(sentence[i]=='"')
                sentence[i]=' ';
        }
        for(i=0;sentence[i]==' ';i++);
        outputfile<<(sentence+i);
    }
    file.close();
    outputfile.close();
    remove(dataname);
    rename(datanametemp,dataname);
}

int getLengthNoStopWords(char sentence[3000])
{
    char word[25],sentencetemp[3000];
    int i,j,k,charcount,notstopcount=0;

    charcount=0,k=0;
    for(i=0;sentence[i]!='\0';i++)
    {
        if(sentence[i]==' '||sentence[i+1]=='\0')
        {
            word[k]='\0';
            int wordFound=0;
            for(j=0;j<stopcount;j++)
            {
                if(strcmpi(word,stopwords[j])==0)
                {
                    wordFound=1;
                    break;
                }
            }
            if(!wordFound)
            {
                notstopcount++;
            }
            k=0;
            charcount=0;
        }
        else
        {
            charcount++;
            word[k++]=sentence[i];
        }
    }
    return notstopcount;
}

void generateInititalReport()
{
    char sentence[3000];
    int i,foundtag;

    std::ifstream file(dataname);
    std::ofstream outputfile(reportname);
    while(!file.eof())
    {
        file.getline(sentence,2999,'.');
        strcat(sentence,".");
        foundtag=0;
        if(getLengthNoStopWords(sentence)>2)
        {
            for(i=0;i<tagcount;i++)
            {
                if(strstr(sentence,tags[i]))
                {
                    foundtag++;
                    outputfile<<(tags[i]+1);
                    outputfile<<"\n";
                    //outputfile<<sentence;
                }
            }
            if(foundtag)
            {
                outputfile<<"$\n";
                if(sentence[0]=='\n'||sentence[0]==' ')
                    outputfile<<(sentence+1);
                else
                    outputfile<<sentence;
            }
        }
    }
    file.close();
    outputfile.close();
}

void cleanReport()
{
    char document[15001],reportnametemp[50];

    strcpy(reportnametemp,cwd);
    strcat(reportnametemp,"\\");
    strcat(reportnametemp,name);
    strcat(reportnametemp,"\\");
    strcat(reportnametemp,"temp.txt");

    //generate file with single sentence
    std::ifstream file(reportname);
    std::ofstream outputfile(reportnametemp);
    while(!file.eof())
    {
        file.getline(document,15000,'.');
        strcat(document,".");
        outputfile<<document;
    }
    file.close();
    outputfile.close();
    remove(reportname);
    rename(reportnametemp,reportname);

    //generate file with separated sentences
    file.open(reportname);
    outputfile.open(reportnametemp);
    while(!file.eof())
    {
        file.getline(document,15000,'.');
        if(strlen(document)>5)
        {
            strcat(document,".\n");
            if(document[0]=='\n')
                outputfile<<(document+1);
            else
                outputfile<<document;
        }
    }
    file.close();
    outputfile.close();
    remove(reportname);
    rename(reportnametemp,reportname);
}

void cleanLinkedin()
{
    char linkedintemp[50],sentence[3000],sentence2[3000];
    int i,j,write;

    strcpy(linkedintemp,cwd);
    strcat(linkedintemp,"\\");
    strcat(linkedintemp,name);
    strcat(linkedintemp,"\\");
    strcat(linkedintemp,"temp.txt");

    std::ifstream file(linkedinname);
    std::ofstream outputfile(linkedintemp);
    while(!file.eof())
    {
        file.getline(sentence,2999);
        strcat(sentence,"\n");
        j=0;
        write=1;
        for(i=0;sentence[i]!='\0';i++)
        {
            if(sentence[i]=='[')
                write=0;
            if(write)
                sentence2[j++]=sentence[i];
            if(sentence[i]==']')
                write=1;
        }
        sentence2[j]='\0';
        if(strlen(sentence2)>3)
            outputfile<<sentence2;
    }
    file.close();
    outputfile.close();
    remove(linkedinname);
    rename(linkedintemp,linkedinname);
}

void cleanMetaData()
{
    char sentence[3000],metadatatemp[50];
    int flag=0;

    strcpy(metadatatemp,cwd);
    strcat(metadatatemp,"\\");
    strcat(metadatatemp,name);
    strcat(metadatatemp,"\\");
    strcat(metadatatemp,"temp.txt");

    //generate file with single sentence
    std::ifstream file(metadataname);
    std::ofstream outputfile(metadatatemp);
    while(!file.eof())
    {
        file.getline(sentence,2999);
        if(!strcmp(sentence,"Signature"))
           break;
        strcat(sentence,"\n");
        if(strlen(sentence)>3)
        {
            if(flag)
            {
                flag=0;
                outputfile<<"\n";
            }
            outputfile<<sentence;
        }
        else
            flag=1;
    }
    file.close();
    outputfile.close();
    remove(metadataname);
    rename(metadatatemp,metadataname);
}

void init()
{
    getcwd(cwd, sizeof(cwd));
    strcpy(dataname,cwd);
    strcat(dataname,"\\");
    strcat(dataname,name);
    strcat(dataname,"\\");
    strcat(dataname,name);
    strcat(dataname,"+data.txt");
    strcpy(metadataname,cwd);
    strcat(metadataname,"\\");
    strcat(metadataname,name);
    strcat(metadataname,"\\");
    strcat(metadataname,name);
    strcat(metadataname,"+metadata.txt");
    strcpy(reportname,cwd);
    strcat(reportname,"\\");
    strcat(reportname,name);
    strcat(reportname,"\\");
    strcat(reportname,name);
    strcat(reportname,"+report.txt");
    strcpy(linkedinname,cwd);
    strcat(linkedinname,"\\");
    strcat(linkedinname,name);
    strcat(linkedinname,"\\");
    strcat(linkedinname,name);
    strcat(linkedinname,"+linkedin.txt");
    getTagsAndStopWords();
}

int main(int argc, char* argv[] )
{
    strcpy(name,argv[1]);
    strcpy(tag,argv[2]);
    init();
    if(!(std::ifstream(dataname)||std::ifstream(linkedinname)))
        crawlData();
    if(std::ifstream(dataname))
    {
        cleanData();
        generateInititalReport();
        cleanReport();
        cleanMetaData();
    }
    if(std::ifstream(linkedinname))
        cleanLinkedin();
}
