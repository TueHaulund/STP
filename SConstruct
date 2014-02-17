import os

env_selection = ARGUMENTS.get('env', 'default')

if env_selection == 'default':
    env_selection = 'msvc' if os.name == 'nt' else 'gcc'

if env_selection == 'gcc':
    print 'Environment: GCC'
    env = Environment(CC = 'g++',
                      CCFLAGS = '-Wall -Wextra -ansi -pedantic -std=gnu++11',
                      ENV = {'PATH' : os.environ['PATH']},
                      TOOLS = ['gnulink','gcc','g++','gas','ar'],
                      CPPPATH = ['./include'])
elif env_selection == 'mingw':
    print 'Environment: MinGW'
    env = Environment(CC = 'g++',
                      CCFLAGS = '-Wall -Wextra -ansi -pedantic -std=gnu++11',
                      ENV = {'PATH' : os.environ['PATH']},
                      TOOLS = ['mingw'],
                      CPPPATH = ['./include'])
elif env_selection == 'msvc':
    print 'Environment: MSVC'
    env = Environment(CC = 'cl',
                      CCFLAGS = '/nologo /EHsc',
                      ENV = {'PATH' : os.environ['PATH']},
                      TOOLS = ['mslink', 'msvc', 'mslib', 'msvs'],
                      CPPPATH = ['./include'])
else:
    print 'Unknown environment, attempting to build with default configuration'
    env = Environment()

SConscript('unittest.scons', variant_dir = './bin', duplicate = 0, exports = 'env')