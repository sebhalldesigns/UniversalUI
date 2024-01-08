#ifndef URMLPARSER_H
#define URMLPARSER_H

#include <fstream>
#include <vector>
#include <cstdio>
#include <string>
#include <map>

// note: general parsing methodology:
// define all acceptable cases, deny every other.
// aka 'whitelist'

enum uRmlTagParseStatus {
    START, // a < was found
    TYPE, // parsing type - from < to the next space character 
    READY, // ready for the next parsing item - this is the state after a type or parameter is set
    KEY, // parsing a parameter key - from a space to the next equals and ignoring whitespace characters
    VALUE, // parsing a parameter value - from one " to the next "
    END, // a / was found after READY

    START_CLOSING, // a / was found after START
    TYPE_CLOSING, // from first alphabetic character to next space
    READY_CLOSING, // if a space is found after a TYPE_CLOSING
    
    COMPLETE // a > was found 
};

struct uRmlTag {
    uRmlTagParseStatus parseStatus = START;
    bool isTerminated = false;
    
    std::string type;

    std::map<std::string, std::string> parameters;

    std::vector<uRmlTag*> children;
    
    // text found between an open and close tag e.g <uLabel>text here</uLabel>
    std::string childText;
};

class uRmlParser {
public:
    static void ParseFile(std::string path) {
        //open file
        std::ifstream infile(path);
        std::vector<char> buffer;

        //get length of file
        infile.seekg(0, infile.end);
        size_t length = infile.tellg();
        infile.seekg(0, infile.beg);

        //read file
        if (length > 0) {
            buffer.resize(length);    
            infile.read(&buffer[0], length);
        }
        
        // chatGPT UTF-8 conversion
        std::vector<uint32_t> codepoints;

        for (std::vector<char>::iterator it = buffer.begin(); it != buffer.end();) {
            uint32_t cp = 0;
            unsigned char lead = *it;
            if (lead < 0x80) {
                cp = *it++;
            } else if ((lead >> 5) == 0x6) {
                cp = (*it++ & 0x1F) << 6;
                cp |= (*it++ & 0x3F);
            } else if ((lead >> 4) == 0xE) {
                cp = (*it++ & 0x0F) << 12;
                cp |= (*it++ & 0x3F) << 6;
                cp |= (*it++ & 0x3F);
            } else if ((lead >> 3) == 0x1E) {
                cp = (*it++ & 0x07) << 18;
                cp |= (*it++ & 0x3F) << 12;
                cp |= (*it++ & 0x3F) << 6;
                cp |= (*it++ & 0x3F);
            } else {
                // Invalid UTF-8 sequence, skip it
                ++it;
                continue;
            }
            codepoints.push_back(cp);
        }

        int lineCount = 0;
        int columnCount = 0;

        uRmlTag* currentTag = nullptr;

        std::vector<uRmlTag*> tags;


        for (uint32_t codepoint : codepoints) {

            // ignore all non-printable characters (ASCII less than 32)
            // and delete (127), unless the charcter is a tab (9) or newline character (10)
            if (codepoint < 32 && !(codepoint == 9 || codepoint == 10) || codepoint == 127) {
                break;
            }
            
            switch (codepoint) {
                case '<':
                    // error if a second < is found before a >
                    if (currentTag == nullptr) {
                        currentTag = new uRmlTag();
                        currentTag->parseStatus = START;
                        printf("allocated a new tag!\n");

                        uRmlTag* candidateParentTag = nullptr;

                        // set candidateParentTag to the deepest of the tree that is still unterminated
                        if (tags.size() > 0 && !tags[tags.size() - 1]->isTerminated) {
                            candidateParentTag = tags[tags.size() - 1];

                            while (candidateParentTag->children.size() > 0) {
                                if (!candidateParentTag->children[candidateParentTag->children.size() - 1]->isTerminated) {
                                    candidateParentTag = candidateParentTag->children[candidateParentTag->children.size() - 1];
                                } else {
                                    break;
                                }
                            }
                        }

                        // add tag either to a nested tag or to the top-level vector
                        if (candidateParentTag != nullptr) {
                            candidateParentTag->children.push_back(currentTag);
                        } else {
                            tags.push_back(currentTag);
                        }

                        printf("done allocating a new tag\n");

                    } else {
                        printf("Error parsing file (at line %d, column %d): an opening tag was detected before a closing tag!\n\t\n", lineCount, columnCount);
                    }

                    printf("<");
                    break;

                case '>':

                    if (currentTag == nullptr) {
                        printf("Error parsing file (at line %d, column %d): a closing tag was detected before an opening tag\n\t\n", lineCount, columnCount);
                    } else {
                        // there is a tag being parsed
                        if (currentTag->parseStatus == END) {
                            // the last parse change was a / so this is the end of a single tag
                            currentTag->isTerminated = true;
                            currentTag->parseStatus = COMPLETE;
                            printf("tag terminated with type %s\n", currentTag->type.c_str());
                            currentTag = nullptr;
                            break;
                        } else if (currentTag->parseStatus == READY || currentTag->parseStatus == TYPE) {

                            currentTag->parseStatus = COMPLETE;
                            printf("tag complete with type %s\n", currentTag->type.c_str());
                            currentTag = nullptr;
                            break;
                        } else if (currentTag->parseStatus == READY_CLOSING || currentTag->parseStatus == TYPE_CLOSING) {
                            // closing tag complete

                            uRmlTag* candidateOpeningTag = nullptr;

                            // set candidateParentTag to the deepest of the tree that is still unterminated
                            if (tags.size() > 0 && !tags[tags.size() - 1]->isTerminated) {
                                candidateOpeningTag = tags[tags.size() - 1];

                                while (candidateOpeningTag->children.size() > 0) {
                                    if (!candidateOpeningTag->children[candidateOpeningTag->children.size() - 1]->isTerminated) {
                                        candidateOpeningTag = candidateOpeningTag->children[candidateOpeningTag->children.size() - 1];
                                    }
                                }
                            }

                            if (candidateOpeningTag == nullptr) {
                                printf("ERROR: tag closed with no corresponding opening tag (at line %d, column %d)\n", lineCount, columnCount );
                            } else if (strcmp(candidateOpeningTag->type.c_str(), currentTag->type.c_str()) != 0) {
                                printf("ERROR: tag closed with a different type (\"%s\") to the corresponding opening tag (\"%s\") (at line %d, column %d) \n", currentTag->type.c_str(), candidateOpeningTag->type.c_str(), lineCount, columnCount);
                            } else {
                                printf("INFO: tag terminated with type \"%s\"\n", currentTag->type.c_str());
                                candidateOpeningTag->isTerminated = true;
                            }

                            currentTag = nullptr;

                        }
                    }

                    printf(">\n");
                    break;
                case ' ':

                    if (currentTag != nullptr) {
                        // i.e in a tag
                        if (currentTag->parseStatus == TYPE) {
                            // end the type if it is being parsed
                            currentTag->parseStatus = READY;
                        } else if (currentTag->parseStatus == TYPE_CLOSING) {
                            currentTag->parseStatus == READY_CLOSING;
                        }
                    }

                    break;
                case '=':
                    printf("=");
                    break;
                case '/':
                    printf("/");
                    if (currentTag != nullptr) {
                        if (currentTag->parseStatus == READY) {
                            currentTag->parseStatus = END;
                        } else if (currentTag->parseStatus == START) {
                            currentTag->parseStatus = START_CLOSING;
                        }
                    }
                    break;
                case '\"':
                    printf("\"");
                    break;
                default:

                    if (currentTag != nullptr) {
                        // i.e in a tag
                        
                        // if its for a type
                        if (currentTag->parseStatus == START) {
                            currentTag->parseStatus = TYPE;
                            printf("%u\n", codepoint);
                            currentTag->type += static_cast<char>(codepoint);
                        } else if (currentTag->parseStatus == TYPE) {
                            printf("%u\n", codepoint);
                            currentTag->type += static_cast<char>(codepoint);
                        } else if (currentTag->parseStatus == START_CLOSING) {
                            currentTag->parseStatus = TYPE_CLOSING;
                            printf("%u\n", codepoint);
                            currentTag->type += static_cast<char>(codepoint);
                        } else if (currentTag->parseStatus == TYPE_CLOSING) {
                            printf("%u\n", codepoint);
                            currentTag->type += static_cast<char>(codepoint);
                        }
                    }

                    //
                    break;

            }

            if (codepoint == '\n') {
                lineCount++;
                columnCount = 0;
            } else {
                columnCount++;
            }

        }


    }

};

#endif // URMLPARSER_H