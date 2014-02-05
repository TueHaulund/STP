import os

env = Environment(CC = 'g++',
	              CCFLAGS = '-Wall -Wextra -ansi -pedantic -std=c++11',
	              ENV = {'PATH' : os.environ['PATH']},
	              TOOLS = ['mingw' if os.name == 'nt' else 'gcc'])

env.Program(target='main', source=['main.cpp'])

