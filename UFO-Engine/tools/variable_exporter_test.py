import UFO_variable_exporter

#UFO_variable_exporter.export_variables("UFO-Engine/tools/variable_exporter_example.h")
source_file_data = UFO_variable_exporter.export_variables("example.h")

print("")
print("class:",source_file_data.klass)
for variable in source_file_data.variables:
    print("Variable name:", variable.name)
    print("type:", variable.vtype)
    print("Alias:", variable.alias)
    print("")