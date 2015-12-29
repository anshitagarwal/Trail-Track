import os
import urllib
import sys
from bs4 import BeautifulSoup
from urllib.parse import urlparse
from urllib.request import urlopen

url="http://www.bing.com/search?q="
#name=input("Enter name:")
name=sys.argv[1]

if not os.path.exists(name):
    os.makedirs(name)
	
finalurl=url+name
html=urlopen(finalurl)
soup=BeautifulSoup(html,"html.parser")
link=""
for tag in soup.findAll('cite'):
	try:
		if 'en.wikipedia' in tag.text:
			link=tag.text
			break
	except:
		pass

try:
	html2=urlopen(link)
	file=open(name+"/"+name+"+data.txt",'w')
	file2=open(name+"/"+name+"+metadata.txt",'w')
	soup2=BeautifulSoup(html2,"html.parser")

	tag=soup2.find('table',{'class':'infobox'})
	write=1
	for t in tag.text:
		if(t=='['):
			write=0
		if(t==']'):
			write=1
			continue
		if(write==1):
			try:
				file2.write(t)
			except:
				file2.write(' ')
	file2.close()
	tag=soup2.findAll('p')
	write=1
	for t1 in tag:
		for t2 in t1.text:
			if(t2=='['):
				write=0
			if(t2==']'):
				write=1
				continue
			if(write==1):
				try:
					file.write(t2)
				except:
					file.write(' ')
		file.write("\n")
	file.close()
except:
	pass

extratag=sys.argv[2]
finalurl2="http://www.bing.com/search?q=linkedin+"
finalurl2=finalurl2+name+"+"+extratag
html=urlopen(finalurl2)
soup=BeautifulSoup(html,"html.parser")
link=""
for tag in soup.findAll('li',{'class':'b_algo'}):
	try:
		tag2=tag.find('h2')
		tag3=tag.find('cite')
		if 'linkedin' in tag3.text:
			if 'profiles' in tag2.text.lower() or 'top' in tag2.text.lower() or 'find' in tag2.text.lower() or 'different' in tag2.text.lower():
				continue
			else:
				link=tag3.text
				break
	except:
		pass
link=link.replace("http://","",1)
link=link.replace("https://","",1)
textiselink="http://www.textise.net/showText.aspx?strURL=https%253A//"+link
try:
	html=urlopen(textiselink)
	file=open(name+"/"+name+"+linkedin.txt",'w')
	soup=BeautifulSoup(html,"html.parser")
	tag=soup.find('div',{'id':'profile'})
	for t in tag:
		try:
			if ("Groups" in t):
				continue
			file.write(t)
			file.write("\n")
			for t2 in t.nextSibling:
				text=t2.text.replace("&apos;","'")
				file.write(text)
				file.write("\n")
			file.write("\n")
		except:
			pass
	file.close()
except:
	pass
