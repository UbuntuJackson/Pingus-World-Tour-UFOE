from project import project

build_succeed = project.build("-ggdb", "", "make" ,"-j6")

#This should only run if build succeeds though?
if build_succeed: project.run()
else: print("Build failed.")