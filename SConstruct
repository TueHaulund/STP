import os

if os.name == 'nt':
	tool = 'mingw'
elif os.name == 'posix':
	tool = 'gcc'
env = Environment(CC = 'g++', CCFLAGS = '-Wall -Wextra -ansi -pedantic -std=c++11', ENV = {'PATH' : os.environ['PATH']}, tools = [tool])
env.Program(target='main', source=['main.cpp'])

