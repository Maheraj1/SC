# File formats in SC

There are currently three types of custom file types in SC:

1. SCMAT **(SC Material)**
1. SCRD **(SC Resource Destination)**
1. SCSRD **(SC Serializable Resource Destination)**
1. SCS  **(SC Scene)**
1. SCMSD **(SC Mono Script Destination)**
1. SCMPS **(SC Multi Platform Shader)**

**Note: This all use the same file type yaml**

# Version Format
- ___major.minor.sub___ followed by reliability indicators. 
sample version string - `1.2.4`, `1.2.5b`, `1.2.6a`.
the postfixes are:
    - alpha - a
    - beta - b
    - None for final


**Here is brief about these formats:**

# SCMAT **(SC Material)**
- SCMAT is used to represent a serialized `SC::Material`
## Format

### color
The Colour/Color of the material

#### Parameters:
- **Vector3 / float[3]** the color in rgb value

### Shader
Id of the shader inside of this object and will be set automatically

### Texture
Id of the texture inside of this object and will be set automatically

# SCRD **(SC Resource Destination)**
SCRD is used to represent a resource (i.e Texture, Shader)

**NOTE: Currently Only Texture is implemented fully**

## Format
### UUID
This should be set automatically and Should not be modified

### Version
The Version of the file. This is used to determine a file format is up to date or not

#### Parameters:
- **String** - Version of file in standard version format

### Type
Shows what type of file it is. i.e 0 = Texture, 1 = Shader
#### Parameters:
- **int** - id of the resource/file type

### Ext
Shows what extension does file have
#### Parameters:
- **String** - extension of the file i.e png



# SCSRD **(SC Serializable Resource Destination)**
SCSRD is used to represent a Serializable resource (i.e Material)

## Format
### UUID
This should be set automatically and Should not be modified

### Version
The Version of the file. This is used to determine a file format is up to date or not

#### Parameters:
- **String** - Version of file in standard version format

### Type
Shows what type of file it is. i.e 0 = General, 1 = Material
#### Parameters:
- **int** - id of the serializable resource/file type

### Ext
Shows what extension does file have
#### Parameters:
- **String** - extension of the file i.e scmat



# SCS **(SC Scene)**
This has to set by `SC::SceneSerializer::SerializeText` or `SC::SceneSerializer::SerializeBinary` **(Binary not Done)**
And has to be set by `SC::SceneSerializer::DeserializeText` or `SC::SceneSerializer::DeserializeBinary` **(Binary not Done)**

# SCMSD **(SC Mono Script Destination)**

`SCMSD` is used to indicate presence of a mono script

## Format

### UUID
- id of the script

### Version
The Version of the file. This is used to determine a file format is up to date or not


# SCMPS **(SC Multi Platform Shader)**
This feature is not implemented and will be added in future updates