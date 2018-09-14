#!/usr/bin/env python2

import urllib2
import os
import mysql.connector
import time 
import datetime

def extract_from_tag(tag,line):
	opener = "<"+tag+">"
	closer = "</"+tag+">"
	try:
	 	i = line.index(opener)
		start = i + len(opener)
		j = line.index(closer,start)
		return line[start:j]
	except ValueError:
		return None

def switcher(num,dictionary,val):
	if num == 2:
	 	dictionary["t1"] =  val
	elif num == 3:
		dictionary["rh1"] =  val
	elif num == 4:
		dictionary["t2"] = val
	elif num == 5:
		dictionary["rh2"] = val
	elif num == 6:
		dictionary["u1"] = val
	elif num == 7:
		dictionary["u2"] = val
	elif num == 8:
		dictionary["p"] = val
	elif num == 9:
		dictionary["co2"] = val
	elif num == 10:
		dictionary["dp1"] = val
	elif num == 11:
		dictionary["dp2"] = val
	else:
		dictionary["default"] = "N/A"


url = "http://ipaddress/values.xml."
fweb = urllib2.urlopen(url)
data = fweb.read()
with open("/tmp/feed.xml","wb") as flocal:
 	flocal.write(data)
flocal.close()
		
filepath = "/tmp/feed.xml"
trh_values = {}

with open(filepath,'rb') as f:
	line = f.readline()
	cnt = 1
	while line:
		if line.find("timestamp") <> -1:
			timestamp = extract_from_tag("timestamp",line)
			a,b = extract_from_tag("timestamp",line).split(" ")
			trh_values["datetime"] = b + " " + a
			line = f.readline()
			cnt += 1
		elif line.find("<v>") <> -1:
			switcher(cnt,trh_values,extract_from_tag("v",line))
			line = f.readline()
			cnt += 1
		else:
			line = f.readline()
f.close()
u2 = float(trh_values["u2"])
co2 = float(trh_values["co2"])
dp2 = float(trh_values["dp2"])
dp1 = float(trh_values["dp1"])
rh1 = float(trh_values["rh1"])
u1 = float(trh_values["u1"])
rh2 = float(trh_values["rh2"])
t2 = float(trh_values["t2"])
t1 = float(trh_values["t1"])
p = float(trh_values["p"])
 
from mysql.connector import errorcode
try:	
 cnx = mysql.connector.connect(user='username', password='password',
                              host='localhost', database = 'databasename')
 cursor = cnx.cursor()
 insert = "INSERT INTO `comet` (`u2`, `co2`, `dp2`, `dp1`, `rh1`, `u1`, `rh2`, `t2`, `t1`, `p`,`datetime`) VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)"
 data = (u2, co2, dp2, dp1, rh1, u1, rh2, t2, t1, p, (trh_values["datetime"]))
 cursor.execute(insert, data)
 cnx.commit()
 cursor.close()
 cnx.close()
except mysql.connector.Error as err:
  if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
    print("Something is wrong with your user name or password")
  elif err.errno == errorcode.ER_BAD_DB_ERROR:
    print("Database does not exist")
  else:
    print(err)
else:
  cnx.close()





