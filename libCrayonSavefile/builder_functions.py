# Needed so we can use scons stuff like builders
from SCons.Script import *

def get_supported_platforms():
	return ['dreamcast', 'pc', 'all']

def my_function(a):
	print("Hello from a function " + a)

# Ch 10.2 has info on multiple values for one arg key value
	# 10.2.2 details help on args
	# Variables('custom.py', ARGUMENTS) means the stuff from ARGUMENTS will override custom.py
	# 10.2.4.1 specifies a variable being boolean, good for debug
	# 10.2.4.2 specifies enums, useful for platform since we only accept those in get_supported_platforms()
	# 10.2.4.3 can be useful for specifying multiple platforms later on. Maybe?
		# It does have a pre-build "all" keyword which does what my all does
		# It also has a none, so make sure to detect that and disable it
	# 10.2.6 is useful for unknown var prevention
def input_handling(args):
	platform = args.get('PLATFORM')
	debug = str(args.get('DEBUG', '0')).lower()	# If var not present, default to False
	if debug == 'true' or debug == '1':
		args['DEBUG'] = True
	if debug == 'false' or debug == '0':
		args['DEBUG'] = False

	# Check if arguments are valid
	supported_platforms = get_supported_platforms()
	if platform == None or platform not in supported_platforms or (args['DEBUG'] != True and args['DEBUG'] != False):
		# Consider using Chapter 9.1, help
		# https://scons.org/doc/production/HTML/scons-user.html#idp140430729969496

		print("""
		Please specify the target platform to compile this project for. You can also
		optionally enable debug flags. By default debug is false.

		eg. `scons PLATFORM=dreamcast DEBUG=false`.
		""")
		print('\tSupported platforms are:')
		for p in supported_platforms:
			print('\t- ' + p)

		exit(1)

	return args

def create_builders(args, paths, program_name):
	import os
	env = list()
	if args['PLATFORM'] == 'dreamcast' or args['PLATFORM'] == 'all':
		print('\tIts: ' + paths['CRAYON_SF_BASE'])
		env.append(Environment(ENV = os.environ, CPPPATH = paths['CRAYON_SF_BASE'], CC = 'kos-cc', CXX = 'kos-c++', AR = 'kos-ar'))

		# Making sure we use the right prefix and suffix
		env[-1]['LIBPREFIX'] = 'lib'
		env[-1]['LIBSUFFIX'] = '.a'
		env[-1]['OBJSUFFIX'] = '.o'	# Windows has .obj
		env[-1]['PROGSUFFIX'] = '.elf'

		# Fix this later, here's a hack for now
		env[-1]['KOS_BASE'] = env[-1]['ENV']['KOS_BASE']
		env[-1]['KOS_GENROMFS'] = env[-1]['ENV']['KOS_GENROMFS']

		# Location of IP.BIN
		env[-1]['IP_DIR'] = paths['IP_BIN'] + 'IP.BIN'

		# Add the platform
		env[-1]['PLATFORM'] = 'dreamcast'
		env[-1]['SPECIFIC_PLATFORM'] = env[-1]['PLATFORM']

	from sys import platform
	if args['PLATFORM'] == 'pc' or args['PLATFORM'] == 'all':
		env.append(Environment(ENV = os.environ, CPPPATH = paths['CRAYON_SF_BASE']))	#Apparently some ppl need that ENV for CCVERSION

		# Add the platform
		env[-1]['PLATFORM'] = 'pc'
		if platform.startswith('linux') == True:
			env[-1]['SPECIFIC_PLATFORM'] = 'linux'
		elif platform == 'win32':
			env[-1]['SPECIFIC_PLATFORM'] = 'windows'
		elif platform == 'darwin':
			env[-1]['SPECIFIC_PLATFORM'] = 'mac'
		else:
			print('Platform "' + platform + '" is not supported')
			exit(1)

	colour_version = [4, 9, 0]

	# Set some env vars for all envs
	for e in env:
		e['CODE_DIR'] = 'code'
		e['CDFS_DIR'] = 'cdfs'
		e['PROG_NAME'] = program_name

		#Add in some cflags if in debug mode
		if args['DEBUG'] == True:
			# Wformat level 2 has extra checks over standard.
			# no-common is where two files define the same global var when they should be seperate
			# g3 is like g, but it includes macro information
			e.AppendUnique(CPPFLAGS = ['-g3', '-Wall', '-Wformat=2', '-fno-common'])

		# Enables GCC colour (Since it normally only does colour for terminals and scons is just an "output")
		# Major, Minor, Patch version numbers
		# We need the CC and CXX checks for pc because this flag is only for GCC/G++
		our_version = list(map(int, e['CCVERSION'].split('.')))
		if all([a >= b for a, b in zip(our_version, colour_version)]) and (e['PLATFORM'] != 'pc' or (e['CC'] == 'gcc' or e['CXX'] == 'g++')):
			e.AppendUnique(CCFLAGS = ['-fdiagnostics-color=always'])

	return env
