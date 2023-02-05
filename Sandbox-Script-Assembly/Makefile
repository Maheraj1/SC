build:
	@dotnet build
	@mv bin/Debug/net6.0/Sandbox-Script-Assembly.dll ../SCEditor/bin/Sandbox-Script-Assembly.dll
	@mv bin/Debug/net6.0/Sandbox-Script-Assembly.pdb ../SCEditor/bin/Sandbox-Script-Assembly.pdb

clean:
	@dotnet clean

add:
	@dotnet add refrence $1

addsln: add
	@dotnet sln add $1