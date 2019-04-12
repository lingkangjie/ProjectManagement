"""
https://arxiv.org/help/api/user-manual
https://www.v2ex.com/t/311247
https://beautifulsoup.readthedocs.io/zh_CN/v4.4.0/
https://www.dataquest.io/blog/web-scraping-tutorial-python/
https://www.crummy.com/software/BeautifulSoup/bs4/doc/
"""

import urllib
import time
#import feedparser
from bs4 import BeautifulSoup
import pickle
import os

# Base api query url
base_url = 'http://export.arxiv.org/api/query?';

# Search parameters
search_query = 'all:machine learning' # search for electron in all fields
start = 0                       # start at the first result
total_results = 20              # want 20 total results
results_per_iteration = 5       # 5 results at a time
wait_time = 3                   # number of seconds to wait beetween calls

print('Searching arXiv for %s' % search_query)

for i in range(start,total_results,results_per_iteration):
    
    print("Results %i - %i" % (i,i+results_per_iteration))
    
    query = 'search_query=%s&start=%i&max_results=%i' % (search_query,
                                                         i,
                                                        results_per_iteration)

    # perform a GET request using the base_url and query
    # with urllib.request.urlopen(base_url+query) as url:
    #    response = url.read()
    response = urllib.request.urlopen(base_url+query).read()

    # parse the response using BeautifulSoup

    if os.path.isfile('./tmp.p')
        with open('./tmp.p','rb') as f:
            soup = pickle.load(f)

    else:
        soup = BeautifulSoup(response, 'html.parser')
        pickle.dump(soup,open('./tmp.p','wb'))

    entries = soup.find_all('entry')
    for entry in entries:
        for c_ in entry.children
        print(type(c_))
        print(c_.string)
"""
    titles = []
    titles_ = soup.find_all('title')
    for title_ in titles:
        titles.append(title.string)

    print(titles)

    authors = []
    authors_ = soup.find_all('author')
    for author_ in authors_:
        for child_ in author_.children:
            authors.append(child_.string)

    print(authors)

    ID = []
    ID_ = soup.find_all('id')
    for id_ in ID_:
        ID.append(id_.string)

    print(ID_)

    publishedData = []
    published_data = soup.find_all('published')
    for p_ in published_data:
        publishedData.append(p_.string)
    print(publishedData)

    updatedDate= []
    updated_date= soup.find_all('updated')
    for u_ in updated_date:
        publishedData.append(u_.string)
    print(updatedDate)

    time.sleep(wait_time)
""" 
