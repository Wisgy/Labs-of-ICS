/*
 * @Author       : Chivier Humber
 * @Date         : 2021-08-30 15:10:31
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-11-23 15:34:30
 * @Description  : file content
 */

#include "assembler.h"

void label_map_tp::AddLabel(const std::string &str, const value_tp &val) {
    labels_.insert(std::make_pair(str, val));
}

value_tp label_map_tp::GetValue(const std::string &str) const {
    // User (vAddress, -1) to represent the error case
    if (labels_.find(str) == labels_.end()) {
        // not found
        return value_tp(vAddress, -1);
    } else {
        return labels_.at(str);
    }
}

std::ostream &operator<<(std::ostream &os, const StringType &item) {
    switch (item) {
    case sComment:
        os << "Comment ";
        break;
    case sLabel:
        os << "Label";
        break;
    case sValue:
        os << "Value";
        break;
    case sOpcode:
        os << "Opcode";
        break;
    case sOprand:
        os << "Oprand";
        break;
    default:
        os << "Error";
        break;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const ValueType &val) {
    switch (val) {
    case vAddress:
        os << "Address";
        break;
    case vValue:
        os << "Value";
        break;
    default:
        os << "Error";
        break;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const value_tp &value) {
    if (value.type_ == vValue) {
        os << "[ " << value.type_ << " -- " << value.val_ << " ]";
    } else {
        os << "[ " << value.type_ << " -- " << std::hex << "0x" << value.val_ << " ]";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const label_map_tp &label_map) {
    for (auto item : label_map.labels_) {
        os << "Name: " << item.first << " " << item.second << std::endl;
    }
    return os;
}

int RecognizeNumberValue(std::string s) {
    // Convert string s into a number
    // TO BE DONE
    int number=0;
    int exp=0;
    if(s[0]=='X'){
        for(int i=s.length()-1;i>0;i--){
            if(s[i]>47&&s[i]<58)number+=int(s[i]-48)*pow(16,exp++);
            else if(s[i]>64&&s[i]<71)number+=int(s[i]-55)*pow(16,exp++);
            else return std::numeric_limits<int>::max();
        }
    }
    else if(s[0]=='#'){
        for(int i=s.length()-1;i>0;i--){
            if(s[i]>47&&s[i]<58)number+=int(s[i]-48)*pow(10,exp++);
            else return std::numeric_limits<int>::max();
        }
    }
    else return std::numeric_limits<int>::max();
    if(number>65535)return std::numeric_limits<int>::max();
    else return number;
}

std::string NumberToAssemble(const int &number) {
    // Convert the number into a 16 bit binary string
    // TO BE DONE
    char num[17];
    int numb=number;
    for(int i=15;i>=0;i--){
        if(numb%2)num[i]='1';
        else num[i]='0';
        numb=numb/2;
    }
    num[16]='\0';
    return std::string(num);
}

std::string NumberToAssemble(const std::string &number) {
    // Convert the number into a 16 bit binary string
    // You might use `RecognizeNumberValue` in this function
    // TO BE DONE
    int num_temp=RecognizeNumberValue(number);
    char s[17];
    for(int i=15;i>=0;i--){
        if(num_temp%2)s[i]='1';
        else s[i]='0';
        num_temp=num_temp/2;
    }
    s[16]='\0';
    return std::string(s);
}

std::string ConvertBin2Hex(std::string bin)  {
    // Convert the binary string into a hex string
    // TO BE DONE
    char s[5];
    for(int i=0 ;i<4;i++){//calculate
        int num_temp=0;
        for(int j=0;j<4;j++)if(bin[i*4+j]=='1')num_temp+=pow(2,3-j);
        switch (num_temp)
        {
        case 0:s[i]='0';break;
        case 1:s[i]='1';break;
        case 2:s[i]='2';break;
        case 3:s[i]='3';break;
        case 4:s[i]='4';break;
        case 5:s[i]='5';break;
        case 6:s[i]='6';break;
        case 7:s[i]='7';break;
        case 8:s[i]='8';break;
        case 9:s[i]='9';break;
        case 10:s[i]='A';break;
        case 11:s[i]='B';break;
        case 12:s[i]='C';break;
        case 13:s[i]='D';break;
        case 14:s[i]='E';break;
        case 15:s[i]='F';break;
        default:
            break;
        }
    }
    s[4]='\0';
    return std::string(s);
}

std::string assembler::TranslateOprand(int current_address, std::string str, int opcode_length) {
    // Translate the oprand
    str = Trim(str);
    auto item = label_map.GetValue(str);
    if (!(item.getType() == vAddress && item.getVal() == -1)) {
        // str is a label
        // TO BE DONE
        int num=item.getVal()-current_address-1;
        char s[opcode_length+1];
        for(int i=opcode_length-1;i>=0;i--){
            if(num%2)s[i]='1';
            else s[i]='0';
            num=num/2;
        }
        s[opcode_length]='\0';
        return std::string(s);
    }
    if (str[0] == 'R') {
        // str is a register
        // TO BE DONE
        switch (str[1]){
        case '0':return "000";
        case '1':return "001";
        case '2':return "010";
        case '3':return "011";
        case '4':return "100";
        case '5':return "101";
        case '6':return "110";
        case '7':return "111";
        default:return "error";
        }
    } else {
        // str is an immediate number
        // TO BE DONE
        int num=RecognizeNumberValue(str);
        char s[opcode_length];
        for(int i=opcode_length-1;i>=0;i--){
            if(num%2)s[i]='1';
            else s[i]='0';
            num=num/2;
        }
        s[opcode_length]='\0';
        return std::string(s);
    }
}

// TODO: add error line index
int assembler::assemble(std::string input_filename, std::string output_filename) {
    // assemble main function
    // parse program

    // store the original string
    std::vector<std::string> file_content;
    std::vector<std::string> origin_file;
    // store the tag for line
    std::vector<LineStatusType> file_tag;
    std::vector<std::string> file_comment;
    std::vector<int> file_address;
    int orig_address = -1;
    std::string line;

    std::ifstream input_file(input_filename);
    if (input_file.is_open()) {
        // Scan #0:
        // Read file
        // Store comments
        while (std::getline(input_file, line)) {
            // Remove the leading and trailing whitespace
            line = Trim(line);
            if (line.size() == 0) {
                // Empty line
                continue;
            }
            std::string origin_line = line;
            // Convert `line` into upper case
            // TO BE DONE
            for(int i=0;i<line.length();i++)
                if(line[i]>=97&&line[i]<=122)line[i]=line[i]-32;
            // Store comments
            auto comment_position = line.find(";");
            if (comment_position == std::string::npos) {
                // No comments here
                file_content.push_back(line);
                origin_file.push_back(origin_line);
                file_tag.push_back(lPending);
                file_comment.push_back("");
                file_address.push_back(-1);
                continue;
            } else {
                // Split content and comment
                // TO BE DONE
                std::string content_str= line.substr(0,comment_position);
                std::string comment_str= line.substr(comment_position,line.length()-comment_position);
                // Delete the leading whitespace and the trailing whitespace
                comment_str = Trim(comment_str);
                content_str = Trim(content_str);
                // Store content and comment separately
                file_content.push_back(content_str);
                origin_file.push_back(origin_line);
                file_comment.push_back(comment_str);
                if (content_str.size() == 0) {
                    // The whole line is a comment
                    file_tag.push_back(lComment);
                } else {
                    file_tag.push_back(lPending);
                }
                file_address.push_back(-1);
            }
        }
    } else {
        std::cout << "Unable to open file" << std::endl;
        // @ Input file read error
        return -1;
    }

    // Scan #1:
    // Scan for the .ORIG & .END pseudo code
    // Scan for jump label, value label, line comments
    int line_address = -1;
    for (int line_index = 0; line_index < file_content.size(); ++line_index) {
        if (file_tag[line_index] == lComment) {
            // This line is comment
            continue;
        }

        auto line = file_content[line_index];

        // * Pseudo Command
        if (line[0] == '.') {
            file_tag[line_index] = lPseudo;
            // This line is a pseudo instruction
            // Only .ORIG & .END are line-pseudo-command
            auto line_stringstream = std::istringstream(line);
            std::string pseudo_command;
            line_stringstream >> pseudo_command;

            if (pseudo_command == ".ORIG") {
                // .ORIG
                std::string orig_value;
                line_stringstream >> orig_value;
                orig_address = RecognizeNumberValue(orig_value);
                if (orig_address == std::numeric_limits<int>::max()) {
                    // @ Error address
                    return -2;
                }
                file_address[line_index] = -1;
                line_address = orig_address;
            } else if (pseudo_command == ".END") {
                // .END
                file_address[line_index] = -1;
                // If set line_address as -1, we can also check if there are programs after .END
                // line_address = -1;
            } else if (pseudo_command == ".STRINGZ") {
                file_address[line_index] = line_address;
                std::string word;
                line_stringstream >> word;
                if (word[0] != '\"' || word[word.size() - 1] != '\"') {
                    // @ Error String format error
                    return -6;
                }
                auto num_temp = word.size() - 1;
                line_address += num_temp;
            } else if (pseudo_command == ".FILL") {
                // TO BE DONE
                file_address[line_index] = line_address++;
            } else if (pseudo_command == ".BLKW") {
                // TO BE DONE
                file_address[line_index] = line_address;
                std::string word;
                line_stringstream >> word;
                int number=RecognizeNumberValue(word);
                if(number==std::numeric_limits<int>::max())return -6;
                line_address += number;
            } else {
                // @ Error Unknown Pseudo command
                return -100;
            }

            continue;
        }

        if (line_address == -1) {
            // @ Error Program begins before .ORIG
            return -3;
        }

        file_address[line_index] = line_address;
        line_address++;

        // Split the first word in the line
        auto line_stringstream = std::stringstream(line);
        std::string word;
        line_stringstream >> word;
        if (IsLC3Command(word) != -1 || IsLC3TrapRoutine(word) != -1) {
            // * This is an operation line
            // TO BE DONE
            file_tag[line_index]=lOperation;
            continue;
        }

        // * Label
        // Store the name of the label
        auto label_name = word;
        // Split the second word in the line
        line_stringstream >> word;
        if (IsLC3Command(word) != -1 || IsLC3TrapRoutine(word) != -1 || word == "") {
            // a label used for jump/branch
            // TO BE DONE
            file_tag[line_index]=lOperation;
            label_map.AddLabel(label_name, value_tp(vAddress, line_address - 1));
        } else {
            file_tag[line_index] = lPseudo;
            if (word == ".FILL") {
                line_stringstream >> word;
                auto num_temp = RecognizeNumberValue(word);
                if (num_temp == std::numeric_limits<int>::max()) {
                    // @ Error Invalid Number input @ FILL
                    return -4;
                }
                if (num_temp > 65535 || num_temp < -65536) {
                    // @ Error Too large or too small value  @ FILL
                    return -5;
                }
                label_map.AddLabel(label_name, value_tp(vAddress, line_address - 1));
            }
            if (word == ".BLKW") {
                // modify label map
                // modify line address
                // TO BE DONE
                line_stringstream >> word;
                auto num_temp = RecognizeNumberValue(word);
                if (num_temp == std::numeric_limits<int>::max()) {
                    // @ Error Invalid Number input @ FILL
                    return -4;
                }
                if (num_temp > 0x7FFF || num_temp < 1) {
                    // @ Error Too large or too small value  @ .BLKW
                    return -5;
                }
                label_map.AddLabel(label_name, value_tp(vAddress, line_address - 1));
                line_address+=num_temp;
            }
            if (word == ".STRINGZ") {
                // modify label map
                // modify line address
                // TO BE DONE
                line_stringstream >> word;
                label_map.AddLabel(label_name, value_tp(vAddress, line_address - 1));
                line_address+=word.length();
            }
        }
    }

    if (gIsDebugMode) {
        // Some debug information
        std::cout << std::endl;
        std::cout << "Label Map: " << std::endl;
        std::cout << label_map << std::endl;

        for (auto index = 0; index < file_content.size(); ++index) {
            std::cout << std::hex << file_address[index] << " ";
            std::cout << file_content[index] << std::endl;
        }
    }

    // Scan #2:
    // Translate

    // Check output file
    if (output_filename == "") {
        output_filename = input_filename;
        if (output_filename.find(".") == std::string::npos) {
            output_filename = output_filename + ".asm";
        } else {
            output_filename = output_filename.substr(0, output_filename.rfind("."));
            output_filename = output_filename + ".asm";
        }
    }

    std::ofstream output_file;
    // Create the output file
    output_file.open(output_filename);
    if (!output_file) {
        // @ Error at output file
        return -20;
    }

    for (int line_index = 0; line_index < file_content.size(); ++line_index) {
        if (file_address[line_index] == -1 || file_tag[line_index] == lComment) {
            // * This line is not necessary to be translated
            continue;
        }

        auto line = file_content[line_index];
        auto line_stringstream = std::stringstream(line);
        if (gIsDebugMode)
            output_file << std::hex << file_address[line_index] << ": ";
        if (file_tag[line_index] == lPseudo) {
            // Translate pseudo command
            
            std::string word;
            line_stringstream >> word;
            if (word[0] != '.') {
                // Fetch the second word
                // Eliminate the label
                line_stringstream >> word;
            }

            if (word == ".FILL") {
                std::string number_str;
                line_stringstream >> number_str;
                auto output_line = NumberToAssemble(number_str);
                if (gIsHexMode)
                    output_line = ConvertBin2Hex(output_line);
                output_file << output_line << std::endl;
            } else if (word == ".BLKW") {
                // Fill 0 here
                // TO BE DONE
                std::string number_str;
                line_stringstream >> number_str;
                int cycle = RecognizeNumberValue(number_str);
                std::string output_line="0000000000000000";
                if (gIsHexMode)
                    output_line = ConvertBin2Hex(output_line);
                for(int i=0;i<cycle;i++)output_file << output_line << std::endl;
            } else if (word == ".STRINGZ") {
                // Fill string here
                // TO BE DONE
                std::string str;
                line_stringstream >> str;
                for(int i=0;i<str.length();i++){
                    auto output_line = NumberToAssemble(int(str[i]));
                    if (gIsHexMode)
                        output_line = ConvertBin2Hex(output_line);
                    output_file << output_line << std::endl;
                }
                if (gIsHexMode)
                    output_file << "0000" << std::endl;
                else output_file << "0000000000000000" << std::endl;
            }

            continue;
        }
        
        if (file_tag[line_index] == lOperation) {
            std::string word;
            line_stringstream >> word;
            if (IsLC3Command(word) == -1 && IsLC3TrapRoutine(word) == -1) {
                // Eliminate the label
                line_stringstream >> word;
            }
            std::string result_line = "";
            auto command_tag = IsLC3Command(word);
            auto parameter_str = line.substr(line.find(word) + word.size());
            parameter_str = Trim(parameter_str);
            
            // Convert comma into space for splitting
            // TO BE DONE
            for(int i=0;i<parameter_str.length();i++)if(parameter_str[i]==',')parameter_str[i]=' ';

            auto current_address = file_address[line_index];

            std::vector<std::string> parameter_list;
            auto parameter_stream = std::stringstream(parameter_str);
            while (parameter_stream >> word) {
                parameter_list.push_back(word);
            }
            auto parameter_list_size = parameter_list.size();
            if (command_tag != -1) {
                // This is a LC3 command
                switch (command_tag) {
                case 0:
                    // "ADD"
                    result_line += "0001";
                    if (parameter_list_size != 3) {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1]);
                    if (parameter_list[2][0] == 'R') {
                        // The third parameter is a register
                        result_line += "000";
                        result_line += TranslateOprand(current_address, parameter_list[2]);
                    } else {
                        // The third parameter is an immediate number
                        result_line += "1";
                        // std::cout << "hi " << parameter_list[2] << std::endl;
                        result_line += TranslateOprand(current_address, parameter_list[2], 5);
                    }
                    break;
                case 1:
                    // "AND"
                    // TO BE DONE
                    result_line += "0101";
                    if (parameter_list_size != 3) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1]);
                    if (parameter_list[2][0] == 'R') {
                        result_line += "000";
                        result_line += TranslateOprand(current_address, parameter_list[2]);
                    } else {
                        result_line += "1";
                        result_line += TranslateOprand(current_address, parameter_list[2], 5);
                    }
                    break;
                case 2:
                    // "BR"
                    // TO BE DONE
                    result_line += "0000000";
                    if (parameter_list_size != 1) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0],9);
                    break;
                case 3:
                    // "BRN"
                    // TO BE DONE
                    result_line += "0000100";
                    if (parameter_list_size != 1) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0],9);
                    break;
                case 4:
                    // "BRZ"
                    // TO BE DONE
                    result_line += "0000010";
                    if (parameter_list_size != 1) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0],9);
                    break;
                case 5:
                    // "BRP"
                    // TO BE DONE
                    result_line += "0000001";
                    if (parameter_list_size != 1) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0],9);
                    break;
                case 6:
                    // "BRNZ"
                    // TO BE DONE
                    result_line += "0000110";
                    if (parameter_list_size != 1) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0],9);
                    break;
                case 7:
                    // "BRNP"
                    // TO BE DONE
                    result_line += "0000101";
                    if (parameter_list_size != 1) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0],9);
                    break;
                case 8:
                    // "BRZP"
                    // TO BE DONE
                    result_line += "0000011";
                    if (parameter_list_size != 1) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0],9);
                    break;
                case 9:
                    // "BRNZP"
                    result_line += "0000111";
                    if (parameter_list_size != 1) {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0], 9);
                    break;
                case 10:
                    // "JMP"
                    // TO BE DONE
                    result_line += "1100000";
                    if (parameter_list_size != 1) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += "000000";
                    break;
                case 11:
                    // "JSR"
                    // TO BE DONE
                    result_line += "01001";
                    if (parameter_list_size != 1) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0],11);
                    break;
                case 12:
                    // "JSRR"
                    // TO BE DONE
                    result_line += "0100000";
                    if (parameter_list_size != 1) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += "000000";
                    break;
                case 13:
                    // "LD"
                    // TO BE DONE
                    result_line += "0010";
                    if (parameter_list_size != 2) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1],9);
                    break;
                case 14:
                    // "LDI"
                    // TO BE DONE
                    result_line += "1010";
                    if (parameter_list_size != 2) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1],9);
                    break;
                case 15:
                    // "LDR"
                    // TO BE DONE
                    result_line += "0010";
                    if (parameter_list_size != 3) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1]);
                    result_line += TranslateOprand(current_address, parameter_list[2],6);
                    break;
                case 16:
                    // "LEA"
                    // TO BE DONE
                    result_line += "1110";
                    if (parameter_list_size != 2) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1],9);
                    break;
                case 17:
                    // "NOT"
                    // TO BE DONE
                    result_line += "1001";
                    if (parameter_list_size != 2) {
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1]);
                    result_line += "111111";
                    break;
                case 18:
                    // RET
                    result_line += "1100000111000000";
                    if (parameter_list_size != 0) {
                        // @ Error parameter numbers
                        return -30;
                    }
                    break;
                case 19:
                    // RTI
                    // TO BE DONE
                    result_line += "1000000000000000";
                    if (parameter_list_size != 0) {
                        // @ Error parameter numbers
                        return -30;
                    }
                    break;
                case 20:
                    // ST
                    result_line += "0011";
                    if (parameter_list_size != 2) {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1], 9);
                    break;
                case 21:
                    // STI
                    // TO BE DONE
                    result_line += "1011";
                    if (parameter_list_size != 2) {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1], 9);
                    break;
                case 22:
                    // STR
                    // TO BE DONE
                    result_line += "1011";
                    if (parameter_list_size != 3) {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1]);
                    result_line += TranslateOprand(current_address, parameter_list[2],6);
                    break;
                case 23:
                    // TRAP
                    // TO BE DONE
                    command_tag = IsLC3TrapRoutine(word);
                    switch (command_tag) {
                        case 0:
                        // x20
                        result_line += "1111000000100000";
                        break;
                        case 1:
                        // x21
                        result_line += "1111000000100001";
                        break;
                        case 2:
                        // x22
                        result_line += "1111000000100010";
                        break;
                        case 3:
                        // x23
                        result_line += "1111000000100011";
                        break;
                        case 4:
                        // x24
                        result_line += "1111000000100100";
                        break;
                        case 5:
                        // x25
                        result_line += "1111000000100101";
                        break;
                        default:
                        // @ Error Unknown command
                        return -50;
                    }
                default:
                    // Unknown opcode
                    // @ Error
                    break;
                }
            } else {
                // This is a trap routine
                command_tag = IsLC3TrapRoutine(word);
                switch (command_tag) {
                    case 0:
                    // x20
                    result_line += "1111000000100000";
                    break;
                    case 1:
                    // x21
                    result_line += "1111000000100001";
                    break;
                    case 2:
                    // x22
                    result_line += "1111000000100010";
                    break;
                    case 3:
                    // x23
                    result_line += "1111000000100011";
                    break;
                    case 4:
                    // x24
                    result_line += "1111000000100100";
                    break;
                    case 5:
                    // x25
                    result_line += "1111000000100101";
                    break;
                    default:
                    // @ Error Unknown command
                    return -50;
                }
            }
            
            if (gIsHexMode)
                result_line = ConvertBin2Hex(result_line);
            output_file << result_line << std::endl;
        }
    }

    // Close the output file
    output_file.close();
    // OK flag
    return 0;
}
