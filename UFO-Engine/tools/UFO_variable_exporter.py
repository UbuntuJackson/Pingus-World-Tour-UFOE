
class Variable:
    def __init__(self, _vtype, _name):
        self.vtype = _vtype
        self.name = _name

def export_variables(_source_file):
    exported_variables = []

    print("Exporting variables from sourcefile", _source_file)

    f = open(_source_file, 'r')
    
    awaits_export = False

    counts_as_new_word = [' ', '\n', '\t', ";", "@", "="]

    datatypes = ["int", "std::string", "float", "double", "bool"]

    current_word = ""

    potential_export = []

    awaits_variable_line = False
    awaited_variable_line = []

    for character in f.read():

        if awaits_variable_line:

            #Searching for datatype
            if len(awaited_variable_line) == 0:
                if not character in counts_as_new_word:
                    current_word += character
                elif current_word != "":
                    if current_word in datatypes:
                        awaited_variable_line.append(current_word)
                    else:
                        current_word = ""

                        potential_export = []

                        awaits_variable_line = False
                        awaited_variable_line = []

                    current_word = ""
                    continue
            
            #Searching for variable name
            elif len(awaited_variable_line) == 1:
                if not character in counts_as_new_word:
                    current_word += character
                elif current_word != "":

                    awaited_variable_line.append(current_word)
                    print(awaited_variable_line)

                    exported_variables.append(Variable(awaited_variable_line[0],awaited_variable_line[1]))

                    #reset all variables
                    current_word = ""

                    potential_export = []

                    awaits_variable_line = False
                    awaited_variable_line = []
            
            continue

        if 'export' in potential_export:
            awaits_variable_line = True
            print(potential_export)
            word = ""
            continue

        if "@" in potential_export:
            
            if current_word == 'export' and character in counts_as_new_word:

                potential_export.append('export')
            
            if len(current_word) >= len('export') and current_word != 'export':
                current_word = ""

                potential_export = []

                awaits_variable_line = False
                awaited_variable_line = []
                continue

            if not character in counts_as_new_word:
                current_word += character
            else:
                current_word = ""

            continue

        if "///" in potential_export:
            #print("character:", character)
            if character == ' ': continue
            if character == '@':
                potential_export.append("@")
                continue
            
            current_word = ""

            potential_export = []

            awaits_variable_line = False
            awaited_variable_line = []

            continue

        if not character in counts_as_new_word:
            current_word += character
            if current_word == "///":
                potential_export.append("///")
                current_word = ""
        elif current_word != "":
            pass

            current_word = ""

    f.close()

    return exported_variables

def add_exported_variables_to_project(_class, _list_of_variables):
    cpp_data_type_to_json_data_type = {
        'int' : 'Int',
        'float' : 'Double',
        'double' : 'Double',
        'bool' : 'Int',
        'std::string' : 'String',
        'string' : 'String'
    }
    
    for variable in _list_of_variables:
        _class.editor_attributes.append((cpp_data_type_to_json_data_type[variable.vtype], variable.name))