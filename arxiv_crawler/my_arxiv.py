"""
Reference:
    https://github.com/joelthchao/arxiv-crawler
    https://arxiv.org/help/api/user-manual
    https://www.crummy.com/software/BeautifulSoup/bs4/doc/
    https://github.com/csurfer/rake-nltk
Usages: 
    In python:
    # import sqlite3
    # conn = sqlite3.connect('arxiv_raw.sqlite')
    # cur = conn.cursor()
    # cur.execute('SELECT * FROM sqlite_master')
    # print(cur.fetchall()) # print the information for all tables

    In terminal:
    $ sqlite3 arxiv_raw.sqlite
    $ sqlite>.header on
    $ sqlite>.mode column
    $ sqlite>SELECT tbl_name FROM sqlite_master WHERE type='table'; // check tables name
    $ sqlite>SELECT * FROM sqlite_master
    $ sqlite>SELECT * FROM Papers
    $ sqlite>SELECT * FROM Authors
    $ sqlite>SELECT * FROM Publications

"""

import urllib
import time
from bs4 import BeautifulSoup
import pickle
import os
import sys
import sqlite3
import nltk
from nltk import FreqDist
from nltk.tokenize import RegexpTokenizer

import matplotlib.pyplot as plt

from rake import Rake

sys.setrecursionlimit(10000)

def base_search(query_url):

    response = urllib.request.urlopen(query_url).read()
    soup = BeautifulSoup(response, 'html.parser')

    """
        if os.path.isfile('./tmp.p'):
        with open('./tmp.p','rb') as f:
        soup = pickle.load(f)

    else:

        # perform a GET request using the base_url and query
        # with urllib.request.urlopen(base_url+query) as url:
        #response = url.read()
        response = urllib.request.urlopen(base_url+query).read()
        # parse the response using BeautifulSoup
        soup = BeautifulSoup(response, 'html.parser')
        pickle.dump(soup,open('./tmp.p','wb'))
        text_file = open('output.txt','w')
        text_file.write(soup.prettify())
        text_file.close()
    """
    entries = soup.find_all('entry')
    entry_elements = ['title','id','published','updated','summary',
    'author','category']
    titles = []
    IDs = []
    published = []
    updated = []
    summary = []
    authors = []
    category = []
    for entry in entries:
        name_ = [] # contains authors name for an entry
        category_ = [] # contains categories for an entry
        for c_ in entry.children:
            if c_ == '\n':
                continue
            elif c_.name == entry_elements[0]:
                titles.append(c_.string)
                continue
            elif c_.name == entry_elements[1]:
                IDs.append(c_.string)
                continue
            elif c_.name == entry_elements[2]:
                published.append(c_.string)
                continue
            elif c_.name == entry_elements[3]:
                updated.append(c_.string)
                continue
            elif c_.name == entry_elements[4]:
                summary.append(c_.string)
                continue
            elif c_.name == entry_elements[5]:
                for n_ in c_.children:
                    if n_.name == 'name':
                        name_.append(n_.string)
                continue
            elif c_.name == entry_elements[6]:
                category_.append(c_['term'])

        authors.append(name_)
        category.append(category_)

    if len(authors) != len(IDs):
        print("number of authors and id mismatch")

    return [titles,IDs,published,updated,summary,authors,category]
    
def search_arxiv():

    # Base api query url
    base_url = 'http://export.arxiv.org/api/query?';

    # Search parameters
    search_query = 'all:machine learning' # search for electron in all fields
    start = 0 # start at the first result
    total_results = 9# want 20 total results
    Vresults_per_iteration = 3 # 5 results at a time
    wait_time = 60 # number of seconds to wait beetween calls

    print('Searching arXiv for %s' % search_query)

    titles = []
    IDs = []
    published = []
    updated = []
    summary = []
    authors = []
    category = []

    for i in range(start,total_results,results_per_iteration):

        print("Results %i - %i" % (i,i+results_per_iteration))

        query = 'search_query=%s&start=%i&max_results=%i' % (search_query,
                                                            i,
                                                            results_per_iteration)

        results = base_search(base_url+search_query)

        titles.append(results[0])
        IDs.append(results[1])
        published.append(results[2])
        updated.append(results[3])
        summary.append(results[4])
        authors.append(results[5])
        category.append(results[6])

        time.sleep(wait_time)

    return [titles,IDs,published,updated,summary,authors,category]

def lasted_arxiv(field,year,month):

    url = 'http://arxiv.org/list/cs.{}/{}{}?show=1000'
    query_url = url.format(field, year, month)

    response = urllib.request.urlopen(query_url).read()
    soup = BeautifulSoup(response, 'html.parser')

    """
    if os.path.isfile('./tmp.p'):
        with open('./tmp.p','rb') as f:
            soup = pickle.load(f)

    else:

        # perform a GET request using the base_url and query
        # with urllib.request.urlopen(base_url+query) as url:
        #response = url.read()
        response = urllib.request.urlopen(query_url).read()
        # parse the response using BeautifulSoup
        soup = BeautifulSoup(response, 'html.parser')
        pickle.dump(soup,open('./tmp.p','wb'))
        text_file = open('tmp.txt','w')
        text_file.write(soup.prettify())
        text_file.close()
    """
    titles = []
    IDs = []
    authors = []

    titles_ = soup.find_all('div', {'class': 'list-title mathjax'})
    for t_ in titles_:
        for tc in t_.children:
            if tc.string !='\n' and tc.string != 'Title:':
                titles.append(tc.string)

    authors_ = soup.find_all('div', {'class': 'list-authors'})
    for a_ in authors_:
        authors_tmp = []
        au_ = a_.find_all('a')
        for aa_ in au_:
            authors_tmp.append(aa_.string)

        authors.append(authors_tmp)

    paper_urls_ = soup.find_all('span', {'class': 'list-identifier'})
    for pu_ in paper_urls_:
        ID = pu_.find('a',{'title':'Abstract'})
        IDs.append(ID.string)

    return (titles,authors,IDs)

def save_DB(fields,year,month,titles,authors,IDs):

    conn = sqlite3.connect('arxiv_raw.sqlite')
    cur = conn.cursor()
    cur.executescript('''
    CREATE TABLE IF NOT EXISTS Papers (
        id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        title TEXT UNIQUE,
        url TEXT,
        year INTEGER,
        month INTEGER
    );
    CREATE TABLE IF NOT EXISTS Authors (
        id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        name TEXT UNIQUE
    );
    CREATE TABLE IF NOT EXISTS Publications (
        paper_id INTEGER, 
        author_id INTEGER,
        PRIMARY KEY (paper_id, author_id)
    );
    ''')

    for title, author, ID in zip(titles, authors, IDs):
        cur.execute('''
            INSERT OR IGNORE INTO Papers (title, url, year, month) 
            VALUES (?, ?, ?, ?)''', (title, ID, int(year), int(month)))
        cur.execute('SELECT id FROM Papers WHERE title = ? ', (title, ))
        paper_id = cur.fetchone()[0]
        
        for name in author:
            cur.execute('''
                INSERT OR IGNORE INTO Authors (name) 
                VALUES (?)''', (name, ))
            cur.execute('SELECT id FROM Authors WHERE name = ? ', (name, ))
            author_id = cur.fetchone()[0]
            cur.execute('''
                INSERT OR REPLACE INTO Publications
                (paper_id, author_id) VALUES (?, ?)''', (paper_id, author_id))
    conn.commit()

def paper_title_NLP(title_corpus):

    # title_corpus is a list of tuple
    # keys like (19,1), means 2019/01
    # value is a list of paper titles after tokenized
    # referece: https://stackoverflow.com/questions/36353125/nltk-regular-expression-tokenizer
    title_dict = {}
    pattern = r'''(?x)            # set flag to allow verbose regexps
            (?:[A-Z]\.)+          # abbreviations, e.g. U.S.A.
            | \w+(?:-\w+)*        # words with optional internal hyphens
            | \$?\d+(?:\.\d+)?%?  # currency and percentages, e.g. $12.40, 82%
            | \.\.\.              # ellipsis
            | [][.,;"'?():_`-]    # these are separate tokens; includes ], [
            '''
    tokenizer = RegexpTokenizer(pattern)
    for t in title_corpus:
        key = (t[3],t[4])
        if key in title_dict:
            filterdText = tokenizer.tokenize(t[1])
            title_dict[key].append(filterdText) 
        else:
            title_dict[key] = []
            filterdText = tokenizer.tokenize(t[1])
            title_dict[key].append(filterdText) 

    # extract keywords with year span
    title_years = {}
    for k,v in title_dict.items():
        key = (k[0],) # year index
        if key in title_years.keys():
            title_years[key].append(v)
        else:
            title_years[key] = []
            title_years[key].append(v)


    deep_freq = []
    for k,v in title_years.items():
        fd = FreqDist()
        vs = [item for sublist in v for item in sublist]
        for v_ in vs:
            for word in v_:
                fd[word] += 1

        print('The keywords for year:20{}'.format(str(k[0])))
        print("Total number of words:{}".format(str(fd.N()))) # total number of samples
        print("Total number of unique words:{}".format(str(fd.B()))) # number of bins or unique samples
        fd.pprint(50) # The maximum number of items to display, default is 10
        deep_freq.append(fd.freq('Deep')+fd.freq('deep'))
        print(deep_freq)

    plt.plot([2012,2013,2014,2015,2016,2017,2018],deep_freq)
    plt.ylabel('frequency of deep word')
    plt.xlabel('years')
    plt.show()

def keyword_title(title_corpus):

    ## here we need NLTK stopwords and punkt, will storaged in /usr/share/nltk_data
    # uncomment to download 

    #nltk.download('stopwords')
    nltk.download('punkt')

    title_dict = {}
    for t in title_corpus:
        key = (t[3],t[4])
        if key in title_dict:
            title_dict[key].append(t[1]) 
        else:
            title_dict[key] = []
            title_dict[key].append(t[1]) 

    # extract keywords with year span
    title_years = {}
    for k,v in title_dict.items():
        key = (k[0],) # year index
        if key in title_years.keys():
            title_years[key].append(v)
        else:
            title_years[key] = []
            title_years[key].append(v)

    for k,v in title_years.items():
        r = Rake()
        vs = [item.rstrip('\n') for sublist in v for item in sublist]
        # a list of strings where each string is a sentence
        #r.extract_keywords_from_sentences(vs)
        #print('The keywords for year:{}'.format(str(k[0])))
        #print(r.get_ranked_phrases_with_scores()[0:10])

        title_txt = '''.'''.join(vs)
        title_txt.strip('\n')
        r.extract_keywords_from_text(title_txt)
        print('The keywords for year:{}'.format(str(k[0])))
        # to get keyword phrases ranked from hightest to lowest with scores
        print(r.get_ranked_phrases_with_scores()[0:10])

if __name__ == '__main__':

    #results = search_query()

    if os.path.isfile('./arxiv_raw_CV_201201_201812.sqlite'):
        print('The database of arxiv has exists')
    else:
        # cs.CV, cs.AI
        fields = 'AI'
        months = ['{:0>2d}'.format(i+1) for i in range(12)]
        years = ['{:0>2d}'.format(i) for i in range(12, 19)]
        #month = '04'
        #year = '19'

        wait_time = 10 # number of seconds to wait beetween calls

        for year in years:
            for month in months:
                print("loading {}-{}".format(year,month)) 
                (titles,authors,IDs) = lasted_arxiv(fields,year,month)
                save_DB(fields, year, month, titles, authors, IDs)
                print("save to DB done")
                time.sleep(wait_time)

    # NLP for arxiv data
    print('NLP for arxiv begin...')
    conn = sqlite3.connect('./arxiv_raw_CV_201201_201812.sqlite')
    cur = conn.cursor()
    cur.execute('SELECT * FROM Papers')
    Papers = cur.fetchall()
    paper_title_NLP(Papers) # base statistics for paper titles
    keyword_title(Papers) # keyword extraction for paper titles
    print('NLP for arxiv end')

