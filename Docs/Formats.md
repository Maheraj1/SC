# File formats in SC

There are currently three types of custom file types in SC:

1. SCRD **(SC Resource Destination)**
2. SCS  **(SC Scene)**
3. SCMPS **(SC Multi Platform Shader)**

**Note: This all use the same file type yaml**

**Here is brief about these formats:**

## SCRD **(SC Resource Destination)**
SCRD is used to represent a resource (i.e Texture, Shader)

**NOTE: Currently Only Texture is implemented fully**

### Format
#### UUID
This should be set automatically and Should not be modified

#### Version
The Version of the file. This is used to determine a file format is up to date or not

##### Parameters:
- **String** - Version of file in ___major.minor.sub___ followed by reliability indicators. 
sample version string - `1.2.4`, `1.2.5b`, `1.2.6a`.
the postfixes are:
    - alpha - a
    - beta - b
    - None for final

#### Type
Shows what type of file it is. i.e 0 = Texture, 1 = Shader
##### Parameters:
- **int** - id of the resource/file type

#### Ext
Shows what extension does file have
##### Parameters:
- **String** - extension of the file i.e png


## SCS **(SC Scene)**
This has to set by `SC::SceneSerializer::SerializeText` or `SC::SceneSerializer::SerializeBinary` **(Binary not Done)**
And has to be set by `SC::SceneSerializer::DeserializeText` or `SC::SceneSerializer::DeserializeBinary` **(Binary not Done)**

## SCMPS **(SC Multi Platform Shader)**
This feature is not implemented and will be added in future updates