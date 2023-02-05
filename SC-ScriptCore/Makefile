build:
	@printf "\e[36mBuilding SC-ScriptCore\e[0m\n"
	@dotnet build SC-ScriptCore/SC-ScriptCore.csproj
	@cp SC-ScriptCore/bin/Debug/net6.0/SC-ScriptCore.dll ../SCEditor/bin/Libs/SC-ScriptCore.dll
	@cp SC-ScriptCore/bin/Debug/net6.0/SC-ScriptCore.pdb ../SCEditor/bin/Libs/SC-ScriptCore.pdb

clean:
	@rm -f ../SCEditor/bin/Libs/SC-ScriptCore.dll ../SCEditor/bin/Libs/SC-ScriptCore.pdb