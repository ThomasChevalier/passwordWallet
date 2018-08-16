"""
option_list_transform by Thomas Chevalier.
(c) 2018 Thomas Chevalier - All rights reserved.
thomasom.chevalier@gmail.com
Usage :
python transform.py [fileName]

[fileName] : a string containing the name of the file containing the option list.
The script will search for the definition of list_options and will write the boring code for you ! Awesome !
"""

import sys
import re
def hrl(line):
	"""
	Adapt the line number to match humans representation
	hrl = human readable line
	"""
	return line+1

def comment_remover(text):
	"""
	Remove comment from a C/C++ string
	Credit to https://stackoverflow.com/a/241506/6599651
	"""
	def replacer(match):
		s = match.group(0)
		if s.startswith('/'):
			return " " # note: a space and not an empty string
		else:
			return s
	pattern = re.compile(
		r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
		re.DOTALL | re.MULTILINE
	)
	return re.sub(pattern, replacer, text)

def final_format(func_name, tokens):
	# base_name = '__auto_gen'
	# for (i,t) in enumerate(tokens):
	# 	args = t.split(',')
	# 	print(func_name+'(', end ='')
	# 	args = [base_name+str(i)] + args
	# 	for arg in args[0:len(args)-1]:
	# 		print(arg, end=', ')
	# 	print(args[-1], ');')
	for (i,t) in enumerate(tokens):
		args = t.split(',')
		if args[0] == "NULL":
			print("""<category str="%s">"""%(args[3]))
		else:
			print("""<action str="%s" func="%s"/>"""%(args[3], args[0]))

def cut():
	print("\n-.-.-.-.-.-.-.-.-.-.-.-.-.-.- cut here -.-.-.-.-.-.-.-.-.-.-.-.-.-.-\n")

argv = sys.argv
if len(argv) != 2:
	print(sys.modules[__name__].__doc__)
	quit()

fileName = argv[1]

try:
	file = open(fileName, "r")
except IOError:
	print("Unable to open file \"%s\"" % (fileName))
	quit()

Found = False
FirstBracket = False
brackets_count = 0

lines = file.readlines()
def_lines = [] # List holding all the lines containing the list definition

for (n,line) in enumerate(lines):
	if "list_options" in line and line[0:2] != "//" and not Found:
		print("Definition found in line %i \"%s\"" % (hrl(n), line.strip()))
		Found = True

	if Found:
		def_lines.append(line)
		if line.count('{') > 0:
			FirstBracket = True
		brackets_count += line.count('{') - line.count('}')
		if brackets_count == 0 and FirstBracket:
			break

block = comment_remover(''.join(def_lines))
block = ''.join([b.strip() for b in block])

tokens = block.split('{')
i = 0
while i < len(tokens):
	if not '}' in tokens[i]:
		del tokens[i]
	else:
		tokens[i] = tokens[i][0:tokens[i].find('}')]
		i += 1

print('%i token%s found' % (len(tokens), 's' if len(tokens) > 1 else ''))
cut()
final_format('DEFINE_NODE', tokens)
cut()
final_format('DECLARE_NODE', tokens)