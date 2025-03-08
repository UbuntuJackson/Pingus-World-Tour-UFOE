import sys

class SourceFileData:
    def __init__(self, _klass, _variables):
        self.klass = _klass
        self.variables = _variables

class Variable:
    def __init__(self, _vtype, _name, _alias = None):
        self.vtype = _vtype
        self.name = _name
        self.alias = _alias
        if _alias == None: self.alias = self.name

def export_variables(_source_file, _class_name_was_user_specified = False) -> SourceFileData:
    exported_variables = []
    exported_classes = []

    print("Exporting variables from sourcefile", _source_file)

    f = open(_source_file, 'r')
    
    awaits_export = False

    keywords = ['export', 'spawn']

    counts_as_new_word = [' ', '\n', '\t', ";", "@", "=", '(', ')']
    counts_as_new_word_class = [' ', '\n', '\t', ";", "@", "=", '(', ')', ':']

    datatypes = ["int", "std::string", "float", "double", "bool"]

    digits = '1234567890.'

    line_count = 1

    current_word = ""

    potential_export = []

    awaits_arguments = False
    awaits_string_argument = False

    awaits_variable_line = False
    awaited_variable_line = []
    
    awaits_parantheses = False

    awaits_class = False
    awaited_class_declaration_line = []

    for character in f.read():

        if character == '\n':
            #print("New line", line_count)
            line_count += 1

        if awaits_class:
            #Searching for datatype
            if len(awaited_class_declaration_line) == 0:
                if not character in counts_as_new_word_class:
                    current_word += character
                elif current_word != "":
                    if current_word == 'class':
                        awaited_class_declaration_line.append(current_word)
                    else:
                        #Reset all variables
                        current_word = ""

                        potential_export = []

                        awaits_arguments = False
                        awaits_string_argument = False

                        awaits_variable_line = False
                        awaited_variable_line = []
                        
                        awaits_parantheses = False

                        awaits_class = False
                        awaited_class_declaration_line = []
                        print("Did not find datatype","Stopped at",character)

                    current_word = ""
                    continue
            
            #Searching for variable name
            elif len(awaited_class_declaration_line) == 1:
                if not character in counts_as_new_word_class:
                    current_word += character
                elif current_word != "":

                    awaited_class_declaration_line.append(current_word)
                    print("Found class:",awaited_class_declaration_line[1], "on line", line_count)
                    exported_classes.append(awaited_class_declaration_line[1])

                    #reset all variables
                    current_word = ""

                    potential_export = []

                    awaits_arguments = False
                    awaits_string_argument = False

                    awaits_variable_line = False
                    awaited_variable_line = []
                    
                    awaits_parantheses = False

                    awaits_class = False
                    awaited_class_declaration_line = []
            
            continue

        if awaits_variable_line:
            #print("Awaits variable line...")

            #Searching for datatype
            if len(awaited_variable_line) == 0:
                if not character in counts_as_new_word:
                    current_word += character
                elif current_word != "":
                    if current_word in datatypes:
                        awaited_variable_line.append(current_word)
                    else:
                        #Reset all variables
                        current_word = ""

                        potential_export = []

                        awaits_arguments = False
                        awaits_string_argument = False

                        awaits_variable_line = False
                        awaited_variable_line = []
                        
                        awaits_parantheses = False
                        print("UFO-ProjectManager: @export was used but datatype was not found.","Line",line_count)

                    current_word = ""
                    continue
            
            #Searching for variable name
            elif len(awaited_variable_line) == 1:
                if not character in counts_as_new_word:
                    current_word += character
                elif current_word != "":

                    awaited_variable_line.append(current_word)

                    if len(potential_export) == 3:
                        exported_variables.append(Variable(awaited_variable_line[0],awaited_variable_line[1]))
                        print("Found variable of type", awaited_variable_line[0], 'with name:', awaited_variable_line[1], "on line", line_count)

                    if len(potential_export) == 4:
                        exported_variables.append(Variable(awaited_variable_line[0],awaited_variable_line[1], potential_export[3]))
                        print("Found variable of type", awaited_variable_line[0], 'with name:', awaited_variable_line[1], "and alias:", potential_export[3], "on line", line_count)

                    #reset all variables
                    current_word = ""

                    potential_export = []

                    awaits_arguments = False
                    awaits_string_argument = False

                    awaits_variable_line = False
                    awaited_variable_line = []
                    
                    awaits_parantheses = False
            
            continue

        if awaits_arguments:

            #While in string submission mode
            if awaits_string_argument:

                if character == '"':
                    #Should await new argument
                    awaits_string_argument = False
                    potential_export.append(current_word)
                    current_word = ""
                    continue

                if character == '\n':
                    
                    print("UFO-ProjectManager: Error: Did not pair quotation marks on line:", line_count)
                    sys.exit()

                current_word += character
                #print(current_word)
            else:
                
                if character == ')':

                    #print("Found closing parantheses")
                    #print(potential_export)
                    awaits_arguments = False
                    awaits_variable_line = True
                    current_word = ""
                    continue

            if character == '"':
                awaits_string_argument = True
                continue
                
            if not awaits_string_argument:
                if character not in digits and character not in counts_as_new_word and character not in [' ',',']:
                    print("UFO-ProjectManager: Error: non literals found in parameters", "Line:", line_count)
                    sys.exit()

            continue

        if awaits_parantheses:
            
            if character == '(':
                awaits_arguments = True
                current_word = ""
                #print("Found open parantheses")
                continue
            
            if character != ' ' and character in counts_as_new_word:
                #print("No arguments at character", character)
                awaits_variable_line = True
                current_word = ""

            continue

        if "@" in potential_export:
            
            if current_word in keywords and character in counts_as_new_word:
                if current_word == 'export':
                    if character == '(':
                        awaits_arguments = True
                    elif character == '\n':
                        awaits_variable_line = True
                    else:
                        #print("Awaits parantheses after export confirmed...","Current character:", character)
                        awaits_parantheses = True
                    potential_export.append(current_word)
                    #print(current_word)
                if current_word == 'spawn':
                    awaits_class = True
                
                current_word = ""
                continue
            
            if character in counts_as_new_word and current_word not in keywords:
                print("UFO-ProjectManager: Did not recognise keyword", current_word, "Line:", line_count)
                print("Keywords include:")
                for kw in keywords:
                    print("    ",kw)
                current_word = ""

                potential_export = []

                awaits_arguments = False
                awaits_string_argument = False

                awaits_variable_line = False
                awaited_variable_line = []
                
                awaits_parantheses = False
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

    if _class_name_was_user_specified:
        return SourceFileData(None,exported_variables)

    if len(exported_classes) == 0:
        print("UFO-ProjectManager: Error: No class found")
        sys.exit()

    if len(exported_classes) > 1:
        print("UFO-ProjectManager: Error: Can't export more than one class")
        sys.exit()

    return SourceFileData(exported_classes[0],exported_variables)

def add_exported_variables_to_project(_class, _list_of_variables):
    cpp_data_type_to_json_data_type = {
        'int' : 'Int',
        'float' : 'Float',
        'double' : 'Float',
        'bool' : 'Int',
        'std::string' : 'String',
        'string' : 'String'
    }
    
    for variable in _list_of_variables:
        _class.editor_attributes.append((cpp_data_type_to_json_data_type[variable.vtype], variable.name, variable.alias))