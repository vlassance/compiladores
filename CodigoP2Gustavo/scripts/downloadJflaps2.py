import re
import urllib.request
import urllib.parse
import html.parser

urltoken = "http://wirth.herokuapp.com/"
url = "http://wirth.herokuapp.com/wirth/"
folder = 'outputlatex/'
response = urllib.request.urlopen(urltoken).read();
token = re.findall(b'value="([^"]*)', response)[1]

urllib.request.urlopen(url).read()
rotate = ["nfa", "dfa", "mdfa", "JFLAP.xml"]
ctr = 0
ctrrot = 0
with open('WIRTH.txt', 'r') as f:
    wirth_notation = f.read();
    values = {
        'authenticity_token' : token,
        'wirth_notation' : wirth_notation,
    }

    data = urllib.parse.urlencode(values)
    data = data.encode('utf-8') 
    req = urllib.request.Request(url, data)
    response = urllib.request.urlopen(req)
    the_page = response.read()
    names = re.findall(b'[^<]*<h3>[^>]*>([^<]*)', the_page)
    for i in re.findall(b'[^<]*<pre>([^<]*)</pre>', the_page):
        with open(folder + names[ctr].decode().replace("_","-") + "." + rotate[ctrrot], 'w') as outfile:
            outfile.write(html.parser.HTMLParser().unescape(i.decode(encoding='UTF-8')).strip())
        ctrrot += 1
        ctrrot %= len(rotate)
        if ctrrot == 0:
            ctr += 1
