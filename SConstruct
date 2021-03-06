import os

env_selection = ARGUMENTS.get('env', 'default')

if env_selection == 'default':
    env_selection = 'mingw' if os.name == 'nt' else 'gcc'

if env_selection == 'gcc':
    print 'Environment: GCC'
    env = Environment(CXX = 'g++',
                      CCFLAGS = '-Wall -Wextra -ansi -pedantic -std=gnu++11 -O3',
                      ENV = {'PATH' : os.environ['PATH']},
                      TOOLS = ['gnulink','gcc','g++','gas','ar'],
                      CPPPATH = ['#/include'])

elif env_selection == 'mingw':
    print 'Environment: MinGW'
    env = Environment(CXX = 'g++',
                      CCFLAGS = '-Wall -Wextra -ansi -pedantic -std=gnu++11 -O3',
                      ENV = {'PATH' : os.environ['PATH']},
                      TOOLS = ['mingw'],
                      CPPPATH = ['#/include'])

elif env_selection == 'msvc':
    print 'Environment: MSVC'
    env = Environment(CXX = 'cl',
                      CCFLAGS = '/nologo /EHsc /Ox',
                      ENV = {'PATH' : os.environ['PATH']},
                      TOOLS = ['mslink', 'msvc', 'mslib', 'msvs'],
                      CPPPATH = ['#/include'])

elif env_selection == 'clang':
    print 'Environment: clang'
    env = Environment(CXX = 'clang++',
                      CCFLAGS = '-Wall -Wextra -ansi -pedantic -std=c++11 -O3',
                      ENV = {'PATH' : os.environ['PATH']},
                      CPPPATH = ['#/include'])

else:
    print 'Unknown environment, attempting to build with default configuration'
    env = Environment()

test = SConscript('test/test.scons', variant_dir = 'test/bin', exports = 'env', duplicate = 0)
example = SConscript('example/example.scons', variant_dir = 'example/bin', exports = 'env', duplicate = 0)

test_alias = env.Alias('test', [test], test[0].path + ' --build_info=yes')
AlwaysBuild(test_alias)

env.Clean(test, '.sconsign.dblite')
env.Default(test)
env.Default(example)
env.Default(test_alias)