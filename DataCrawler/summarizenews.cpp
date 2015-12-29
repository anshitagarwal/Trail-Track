#include<stdio.h>
#include<string.h>
#include<fstream>
#include<cstdlib>
#include<conio.h>
#include<iostream>
#include <unistd.h>

char stopwords[1500][25],tags[200][20],name[50],newsname[500],newsreportname[500],tag[50],cwd[500];
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

void crawlNews()
{
    int i;
    char command[500]="C:\\Python35\\python ";
    strcat(command,cwd);
    strcat(command,"\\DataCrawler\\crawlnews.py ");

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

void generateNewsReport()
{
    char sentence[30000],sentence2[3000],source[500];
    int i,j,k,foundtag;

    std::ifstream file(newsname);
    std::ofstream outputfile(newsreportname);
    while(!file.eof())
    {
        file.getline(sentence,29999,'^');
        file.getline(source,499,'^');
        if(file.eof())
            break;
        outputfile<<source;
        outputfile<<"^\n";
        i=0;
        while(sentence[i]!='\0')
        {
            int flag=0;
            for(j=0;sentence[i]!='.'&&sentence[i]!='\0';i++)
            {
                flag=1;
                sentence2[j++]=sentence[i];
            }
            if(flag)
            {
                sentence2[j++]='.';
                sentence2[j++]='\n';
                sentence2[j]='\0';
                foundtag=0;
                if(getLengthNoStopWords(sentence2)>2)
                {
                    for(k=0;k<tagcount;k++)
                    {
                        if(strstr(sentence2,tags[k]))
                            foundtag++;
                    }
                    if(foundtag)
                    {
                        if(sentence2[0]=='\n'||sentence2[0]==' ')
                            outputfile<<(sentence2+1);
                        else
                            outputfile<<sentence2;
                    }
                }
            }
            if(sentence[i]=='\0'||sentence[i+1]=='\0')
                break;
            i++;
        }
        outputfile<<"\n^";
    }
    file.close();
    outputfile.close();
}
void cleanNewsReport()
{
    char source[1000],sentence[3000],newsreporttemp[50];
    int k,foundtag,flag=0;

    strcpy(newsreporttemp,cwd);
    strcat(newsreporttemp,"\\");
    strcat(newsreporttemp,name);
    strcat(newsreporttemp,"\\");
    strcat(newsreporttemp,"temp.txt");

    //generate file with single sentence
    std::ifstream file(newsreportname);
    std::ofstream outputfile(newsreporttemp);
    while(!file.eof())
    {
        file.getline(sentence,2999);
        if(sentence[strlen(sentence)-1]=='^')
        {
            outputfile<<sentence;
            outputfile<<"\n";
            continue;
        }
        strcat(sentence,"\n");
        if(strlen(sentence)>3)
        {
            foundtag=0;
            if(getLengthNoStopWords(sentence)>2)
            {
                for(k=0;k<tagcount;k++)
                {
                    if(strstr(sentence,tags[k]))
                        foundtag++;
                }
                if(foundtag)
                {
                    if(sentence[0]=='\n'||sentence[0]==' ')
                        outputfile<<(sentence+1);
                    else
                        outputfile<<sentence;
                }
            }
        }
    }
    file.close();
    outputfile.close();
    remove(newsreportname);
    rename(newsreporttemp,newsreportname);
}
void init()
{
    getcwd(cwd, sizeof(cwd));
    strcpy(newsname,cwd);
    strcat(newsname,"\\");
    strcat(newsname,name);
    strcat(newsname,"\\");
    strcat(newsname,name);
    strcat(newsname,"+news.txt");
    strcpy(newsreportname,cwd);
    strcat(newsreportname,"\\");
    strcat(newsreportname,name);
    strcat(newsreportname,"\\");
    strcat(newsreportname,name);
    strcat(newsreportname,"+newsreport.txt");
    getTagsAndStopWords();
}

int main(int argc, char* argv[] )
{
    strcpy(name,argv[1]);
    strcpy(tag,argv[2]);
    init();
    if(!std::ifstream(newsname))
        crawlNews();
    if(std::ifstream(newsname))
    {
        generateNewsReport();
        cleanNewsReport();
    }
}
