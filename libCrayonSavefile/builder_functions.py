# Needed so we can use scons stuff like builders
from SCons.Script import *

def get_supported_platforms():
	return ['dreamcast', 'pc', 'all']

def get_supported_platforms_old():
	return ['dreamcast', 'pc']

# This will prevent 'none' from working
def valid_platform(key, val, env):
	# Split val so we can check all arguments
	for v in val.split():
		if not v in get_supported_platforms():
			print("Please give a value for PLATFORMS. Type \"scons --help\" for more information")
			Exit(1)

def input_logic(args):
	# vars = Variables('#/scons_args.py', args)
	# vars = Variables('scons_args.py', args)
	vars = Variables(None, args)
	vars.AddVariables(
		BoolVariable('DEBUG',
					help = "Build in debug mode",
					default = 0),
	)

	# This is the only way to set a validator for ListVariable. LHS is tuple
	(key, help, default, _, converter) = ListVariable('PLATFORMS',
		help = "The platforms we want to build",
		default = 'unspecified',
		names = get_supported_platforms()
	)	
	vars.Add((key, help, default, valid_platform, converter))

	# Create a processing env to handle unknown params
	processing_env = Environment(tools = [], variables = vars)
	unknown_params = vars.UnknownVariables()	# Must be done after env creation
	if unknown_params:
		print("Invalid options detected: " + ', '.join(map(str, unknown_params.keys())))
		Exit(1)

	Help(vars.GenerateHelpText({}))	# It doesn't actually properly look at the parameter...

	return vars

# Ch 10.2 has info on multiple values for one arg key value
	# 10.2.2 details help on args
	# Variables('custom.py', ARGUMENTS) means the stuff from ARGUMENTS will override custom.py

# 		# Consider using Chapter 9.1, help
# 		# https://scons.org/doc/production/HTML/scons-user.html#idp140430729969496


def create_builders(params, our_vars):
	# Split the PLATFORMS into a list
	params_env = Environment(tools = [], variables = params)
	target_platforms = str(params_env['PLATFORMS']).split(',')

	# If all was present, just set it to all platforms
	if 'all' in target_platforms:
		target_platforms = get_supported_platforms()

	import os
	env = list()

	# How do I check the params since its not a list?
	if 'dreamcast' in target_platforms:
		env.append(
			Environment(
				variables = params,
				ENV = os.environ,
				CC = 'kos-cc',
				CXX = 'kos-c++',
				AR = 'kos-ar',
			)
		)

		# Making sure we use the right prefix and suffix
		env[-1]['LIBPREFIX'] = 'lib'
		env[-1]['LIBSUFFIX'] = '.a'
		env[-1]['OBJSUFFIX'] = '.o'	# Windows has .obj
		env[-1]['PROGSUFFIX'] = '.elf'

		# Fix this later, here's a hack for now
		env[-1]['KOS_BASE'] = env[-1]['ENV']['KOS_BASE']
		env[-1]['KOS_GENROMFS'] = env[-1]['ENV']['KOS_GENROMFS']

		# Location of IP.BIN
		if 'IP_BIN_DIR' in our_vars:
			env[-1]['IP_BIN_DIR'] = our_vars['IP_BIN_DIR'] + 'IP.BIN'

		# Add the platform
		env[-1]['GENERAL_PLATFORM'] = 'dreamcast'
		env[-1]['SPECIFIC_PLATFORM'] = env[-1]['GENERAL_PLATFORM']

	from sys import platform
	if 'pc' in target_platforms:
		# Apparently some ppl need os' ENV for CCVERSION
		env.append(
			Environment(
				variables = params,
				ENV = os.environ,
			)
		)

		# Add the platform
		env[-1]['GENERAL_PLATFORM'] = 'pc'
		if platform.startswith('linux') == True:
			env[-1]['SPECIFIC_PLATFORM'] = 'linux'
		elif platform == 'win32':
			env[-1]['SPECIFIC_PLATFORM'] = 'windows'
		elif platform == 'darwin':
			env[-1]['SPECIFIC_PLATFORM'] = 'mac'
		else:
			print('Platform "' + platform + '" is not supported')
			Exit(1)

	colour_version = [4, 9, 0]

	# Set some env vars for all envs
	for e in env:
		# Ensure CRAYON_SF_BASE is set
		if 'CRAYON_SF_BASE' in our_vars:
			e['CRAYON_SF_BASE'] = our_vars['CRAYON_SF_BASE']
			# e.AppendUnique(CPPPATH = ['$CRAYON_SF_BASE'])	# Doesn't seem to be nessisary, but keeping just incase
			e.AppendUnique(CPPPATH = ['$CRAYON_SF_BASE/include/'])
		else:
			print('CRAYON_SF_BASE is missing, please add the path')
			Exit(1)

		e['CODE_DIR'] = 'code'
		e['CDFS_DIR'] = 'cdfs'
		if 'PROG_NAME' in our_vars:
			e['PROG_NAME'] = our_vars['PROG_NAME']

		#Add in some cflags if in debug mode
		if e['DEBUG'] == True:
			# Wformat level 2 has extra checks over standard.
			# no-common is where two files define the same global var when they should be seperate
			# g3 is like g, but it includes macro information
			e.AppendUnique(CPPFLAGS = ['-g3', '-Wall', '-Wformat=2', '-fno-common'])

		# Enables GCC colour (Since it normally only does colour for terminals and scons is just an "output")
		# Major, Minor, Patch version numbers
		# We need the CC and CXX checks for pc because this flag is only for GCC/G++
		our_version = list(map(int, e['CCVERSION'].split('.')))
		if all([a >= b for a, b in zip(our_version, colour_version)]) and (e['GENERAL_PLATFORM'] != 'pc' or (e['CC'] == 'gcc' or e['CXX'] == 'g++')):
			e.AppendUnique(CCFLAGS = ['-fdiagnostics-color=always'])

	return env
