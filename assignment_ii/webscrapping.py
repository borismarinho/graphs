import requests
import bs4
import re
import time
import unidecode

codeRegex = re.compile(r'\d\d\d\d\d\d')
creditsRegex = re.compile(r'\d\d\d \d\d\d \d\d\d \d\d\d')


def getURLs(url):
    downloadHtml = requests.get(url)
    soup = bs4.BeautifulSoup(downloadHtml.text, 'html.parser')
    classURLs = []
    i = 1
    for link in soup.find_all('a'):
        if "/graduacao/disciplina.aspx?cod=" in link.get('href'):
            # print(str(i) + " https://matriculaweb.unb.br" + link.get('href'))
            # i += 1
            if '116912' not in link.get('href') and '116921' not in link.get('href'):
                classURLs.append("https://matriculaweb.unb.br" + link.get('href'))
            # downloadHtml_2 = requests.get("https://matriculaweb.unb.br" + link.get('href'))
            # soup2 = bs4.BeautifulSoup(download_Html2.text, 'html.parser')
            # time.sleep(1)
    print(classURLs)
    return classURLs

def getCodes(url):
    downloadHtml = requests.get(url)
    soup = bs4.BeautifulSoup(downloadHtml.text, 'html.parser')
    classes = []
    for table in soup.find_all(id="datatable"):
        for line in table:
            try:
                code = codeRegex.search(line.getText())
                code = code.group()
                code = int(code)
                if code != 116921 and code != 116912:
                    classes.append(code)
            except:
                pass
    return classes


def getCodesAndCredits(url, disciplines):
    downloadHtml = requests.get(url)
    soup = bs4.BeautifulSoup(downloadHtml.text, 'html.parser')
    for table in soup.find_all(id="datatable"):
        for line in table:
            try:
                code = codeRegex.search(line.getText())
                code = code.group()
                code = int(code)
                if code != 116921 and code != 116912:
                    credits = creditsRegex.search(line.getText())
                    credits = credits.group()
                    credits = credits.split()
                    credits = [int(credits[0]), int(credits[1]), int(credits[2]), int(credits[3])]
                    credits = credits[0] + credits[1] + credits[2]
                    disciplines[code] = ['', credits, [], []]
            except:
                pass
    return disciplines


def evalueatePreReqs(classURL):
    downloadHtml = requests.get(classURL)
    soup = bs4.BeautifulSoup(downloadHtml.text, 'html.parser')
    pRList = []
    # flag = False
    # preReq = ''
    preReqsNamesList = []
    for table in soup.find_all(id="datatable"):
        for line in table:
            if "Denominação" in line.getText():
                className = line.getText()[11:]
                # print(className)
            if "Pré-requisitos" in line.getText():
                classPreReqs = line.getText()[14:]
                preReqsList = codeRegex.findall(classPreReqs)
                for i in preReqsList:
                    pRList.append(int(i))
                # classPreReqs = classPreReqs.split()
                # print(preReqsList)
                # print(classPreReqs)
        #         for word in classPreReqs:
        #             if flag:
        #                 preReq = preReq + ' ' + word
        #             if word in preReqsList:
        #                 preReq = word
        #                 flag = True
        #             if word == 'OU' or word == 'E':
        #                 flag = False
        #                 preReqsNamesList.append(preReq)
        #                 preReq = ''
        #         preReqsNamesList.append(preReq)
        # print(preReqsNamesList)
    return (className, pRList)

def setBlocks(disciplines, code):
    for j in disciplines[code][2]:
        disciplines[j][3].append(code)
        # print(disciplines[j])
    return disciplines


def mountGML(disciplines, codes, j):
    fp = open("cic.txt", "w+")
    i = 1
    fp.write("graph\n")
    fp.write("[\n")
    for code in disciplines:
        fp.write("  node\n")
        fp.write("  [\n")
        fp.write("    id " + str(code) + "\n")
        fp.write("    label " + unidecode.unidecode(disciplines[code][0]).encode('ascii', 'ignore').decode('ascii') + "\n")
        fp.write("    weight " + str(disciplines[code][1]) + "\n")
        fp.write("  ]\n")
    for code in disciplines:
        #if len(disciplines[code][3]) != 0:
        for target in disciplines[code][3]:
            fp.write("  edge\n")
            fp.write("  [\n")
            fp.write("    source " + str(code) + "\n")
            fp.write("    target " + str(target) + "\n")
            fp.write("  ]\n")
    fp.write("]")
    fp.close()
urls = []
codes = []
preReqs = []
disciplines = {}
j = 0
urls = getURLs('https://matriculaweb.unb.br/graduacao/fluxo.aspx?cod=1856')
time.sleep(1)
codes = getCodes('https://matriculaweb.unb.br/graduacao/fluxo.aspx?cod=1856')
time.sleep(1)
codesAndCredits = getCodesAndCredits('https://matriculaweb.unb.br/graduacao/fluxo.aspx?cod=1856', disciplines)
time.sleep(1)
for i in range(len(urls)):
    preReqs.append(evalueatePreReqs(urls[i]))
    codesAndCredits[codes[i]][0] = preReqs[i][0]
    codesAndCredits[codes[i]][2] = preReqs[i][1]
    codesAndCredits[codes[i]][2] = list(set(codesAndCredits[codes[i]][2]).intersection(set(codes)))
    print(codesAndCredits[codes[i]])
    disciplines = setBlocks(codesAndCredits, codes[i])
    print(disciplines)
    time.sleep(1)

mountGML(codesAndCredits, codes[i], j)
j += 1
