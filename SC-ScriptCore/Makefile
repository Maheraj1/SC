build:
	@printf "\e[36mBuilding SC-ScriptCore\e[0m\n"
	@dotnet build SCEngine/SCEngine.csproj
	@cp SCEngine/bin/Debug/net6.0/SCEngine.dll ../SCEditor/bin/Libs/SCEngine.dll
	@cp SCEngine/bin/Debug/net6.0/SCEngine.pdb ../SCEditor/bin/Libs/SCEngine.pdb

clean:
	@rm -f ../SCEditor/bin/Libs/SCEngine.dll ../SCEditor/bin/Libs/SCEngine.pdb