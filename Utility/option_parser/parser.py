"""
option_parser by Thomas Chevalier.
(c) 2018 Thomas Chevalier - All rights reserved.
thomasom.chevalier@gmail.com
Usage :
python parser.py [fileName]

[fileName] : The filename of the xml file describing the option list
"""

import sys
import xml.etree.ElementTree as ET
import datetime

def parse_elem(elem):
	"""
	Returns a list with all the objects parsed.
	The list contains tuple.
	Each tuple represent either a category, an action or a stop
	category : {'id': string, 'str':string, 'children':list}
	action   : {'id': string, 'str':string, 'func':string}
	stop     : {'id': string}
	"""

	def getAlias(elem):
		try:
			a = elem.attrib['alias']
		except:
			a = ""
		return a
	nodes = []
	for child in elem:
		if child.tag == 'category':
			try:
				name = child.attrib['str']
			except:
				print('Missing or invalid attribute.')
				quit()
			nodes.append({'id':'category', 'str':name, 'children':parse_elem(child)})

		elif child.tag == 'action':
			try:
				name = child.attrib['str']
				func = child.attrib['func']
			except:
				print('Missing or invalid attribute.')
				quit()
			nodes.append({'id':'action', 'str':name, 'func':func})
		elif child.tag == 'stop':
			nodes.append({'id':'stop'})
		else:
			continue
		if getAlias(child):
			nodes[-1]['alias'] = getAlias(child)
	return nodes

def structure(info):
	def offset(item, o):
		if item['child'] >= 0:
			item['child'] = item['child'] + o
		if item['neighbour'] >= 0:
			item['neighbour'] = item['neighbour'] + o
		return item

	structured = []
	set_neighbour = -1
	for (i,item) in enumerate(info):
		if item['id'] == 'stop':
			if i == 0:
				fatal("Invalid position of <stop/>")
			else:
				try:
					structured[-1]["neighbour"] = -2
				except:
					fatal('Invalid <stop/> placement.')

		elif item['id'] == 'category':

			# Update neighbour
			try:
				if set_neighbour != -1 and structured[set_neighbour]['neighbour'] != -2:
					structured[set_neighbour]['neighbour'] = len(structured)
			except:
				print(set_neighbour)

			set_neighbour = len(structured)
			structured.append({'id':'category', 'child':len(structured)+1, 'neighbour':-1, 'str':item['str']})

			# Update alias
			try:
				structured[-1]['alias'] = item['alias']
			except:
				pass

			sub_structure = structure(item['children'])
			structured += [ offset(it, len(structured)) for it in sub_structure]

		elif item['id'] == 'action':
			if set_neighbour != -1 and structured[set_neighbour]['neighbour'] != -2:
				structured[set_neighbour]['neighbour'] = len(structured)
			set_neighbour = len(structured)

			structured.append({'id':'action', 'child':-1, 'neighbour':-1, 'str':item['str'], 'func':item['func']})

			# Update alias
			try:
				structured[-1]['alias'] = item['alias']
			except:
				pass
	try:
		structured[-1]['neighbour'] = -2
	except:
		pass
	return structured

def readable(struct, what):
	base_name = '__auto'
	strs = []

	for (i,s) in enumerate(struct):
		try:
			func = s['func']
		except:
			func = 'NULL'
		c = str(s['child']) if s['child'] >= 0 else 'OPTIONS_LIST_NONE'
		n = str(s['neighbour']) if s['neighbour'] >= 0 else 'OPTIONS_LIST_NONE'
		i_str = '0'+str(i) if i < 10 else str(i)
		strs.append('/* %s */    %s(%s, %s,%s, %s, STR(%s));'%(i_str, what, base_name+str(i), func, c, n, s['str']))
	return strs

def fatal(str):
	print(str)
	quit()

def format_array(struct):
	strs = []
	base_name = '__auto'
	for (i,s) in enumerate(struct):
		strs.append("\t&%s," % (base_name+str(i)))
	return strs

argv = sys.argv
if len(argv) != 2:
	print(sys.modules[__name__].__doc__)
	quit()

fileName = argv[1]

try:
	tree = ET.parse(fileName)
except:
	fatal('Invalid file \"%s\"' % (fileName))

root = tree.getroot()

raw_info = parse_elem(root)
processed = structure(raw_info)

now = datetime.datetime.now()

# Check for aliases
aliases = []
for (i, item) in enumerate(processed):
	try:
		aliases.append('#define %s (%i)' % (item['alias'], i))
	except:
		pass

header = """/*
 * File auto-generated by option_parser.
 * Copyright - Thomas Chevalier - %i.
 * thomasom.chevalier@gmail.com
 * File created on %s
*/

#ifndef OPTIONS_LIST_HEADER_THOMAS_CHEVALIER
#define OPTIONS_LIST_HEADER_THOMAS_CHEVALIER

#include <stdint.h>
#include <avr/pgmspace.h>

#include "../Graphics/String.h"

#include "Options_Callback_Password.h"
#include "Options_Callback_Sort.h"
#include "Options_Callback_Advanced.h"

#define OPTIONS_LIST_NUM_OPTIONS (%i)
#define OPTIONS_LIST_NONE OPTIONS_LIST_NUM_OPTIONS

#ifndef NULL
#define NULL ((void*)0)
#endif

#define OPTIONS_LIST_MAX_INTRICATION (2)

#define STR(str) str##_index

typedef struct _Option_Node
{
	void (*callback)(void);  //< Function to call when a final node is selected, NULL if their is children, not NULL otherwise
	uint8_t child;
	uint8_t neighbour;
	uint16_t str;
} Option_Node;

#define DEFINE_NODE(name, callback, child, neighbour, strIndex) extern const __flash Option_Node name
#define DECLARE_NODE(name, callback, child, neighbour, strIndex) const __flash Option_Node name = {callback, child, neighbour, strIndex}

// Aliases
%s

// Data
%s

// Array
extern const __flash Option_Node* const __flash list_options [OPTIONS_LIST_NUM_OPTIONS];

#endif // OPTIONS_LIST_HEADER_THOMAS_CHEVALIER
""" % (now.year, str(now), len(processed), '\n'.join(aliases), '\n'.join(readable(processed, 'DEFINE_NODE')))


headerFile = open("Options_List.h", "w")
headerFile.write(header)
headerFile.close()
source = """/*
 * File auto-generated by option_parser.
 * Copyright - Thomas Chevalier - %i.
 * thomasom.chevalier@gmail.com
 * File created on %s
*/

#include "Options_List.h"

// Data definition
%s

// Array
const __flash Option_Node* const __flash list_options [OPTIONS_LIST_NUM_OPTIONS]= 
{
%s
};

""" % (now.year, str(now), '\n'.join(readable(processed, 'DECLARE_NODE')), '\n'.join(format_array(processed)))

sourceFile = open("Options_List.cpp", "w")
sourceFile.write(source)
sourceFile.close()