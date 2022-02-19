#pragma once
#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <variant>
#include <initializer_list>
#include <cassert>

namespace std
{
	template<typename Ty>
	constexpr const Ty& get(void* ptr)
	{
		return *reinterpret_cast<Ty*>(ptr);
	}
}

namespace SJson
{
	// Type declarations
	// 
	class JsonNode;
	class JsonConvert;
	struct JsonToken;
	struct JsonFormatOption;

	enum class ValueType : uint8_t
	{
		Null,           // Null value: null
		Object,         // Object value: unordered set of name - value pairs
		Array,          // Array value: ordered array of values
		String,         // String value: "hello world"
		Boolean,        // Boolean value: true/false
		Integer,        // Integer value: long long int
		Float,          // Float value: floating points 3.14159

		__COUNT
	};

	enum class TokenType : uint8_t
	{
		Null,           // Token: "null"
		True,			// Token: "true"
		False,			// Token: "false"
		Integer,        // Token: (+|-)?[0-9]+ 
		Float,			// Token: (+|-)?[0-9]+.[0-9]+ / (+|-)?[0-9]+.[0-9]+e(+|-)?[0-9]+
		String,			// Token: "\".*\""
		LeftBrace,		// Token: "{"
		RightBrace,		// Token: "}"
		LeftBracket,	// Token: "["  
		RightBracket,   // Token: "]"
		Comma,			// Token: ","
		Colon,			// Token: ":"
		Comment,		// Token: "#"
		EndOfFile,		// Token: EOF
		Unknown,		// error token

		__COUNT
	};

	using array_type = std::vector<JsonNode>;
	using object_type = std::map<std::string, JsonNode>;
	using array_type_init = std::initializer_list<JsonNode>;
	using object_type_init = std::initializer_list<std::pair<const std::string, JsonNode>>;

	using JsonValue = std::variant<
		std::monostate,
		int64_t,
		double,
		bool,
		std::string,
		array_type,
		object_type
	>;

	template <typename T, typename U>
	using t_enable_if_same_type = std::enable_if_t<std::is_same<std::decay_t<T>, U>::value, nullptr_t>;

	template <typename T>
	using t_enable_if_integral_type = std::enable_if_t<std::is_integral<std::decay_t<T>>::value && !std::is_same<std::decay_t<T>, bool>::value, nullptr_t>;

	template <typename T>
	using t_enable_if_floating_type = std::enable_if_t<std::is_floating_point<std::decay_t<T>>::value, nullptr_t>;

	template <typename T>
	using t_enable_if_string_type = std::enable_if_t<std::is_floating_point<std::decay_t<T>>::value, nullptr_t>;

	/**
	 * @brief
	 * @param type
	 * @return
	*/
	std::string GetValueTypeName(ValueType type);

	/**
	 * @brief
	 * @param list
	 * @return
	*/
	JsonNode object(std::initializer_list<std::pair<const std::string, JsonNode>> list);

	/**
	 * @brief
	 * @param list
	 * @return
	*/
	JsonNode array(std::initializer_list<JsonNode> list);


	/**
	 * @brief
	 * @param tokens
	 * @param index
	 * @param newIndex
	 * @return
	*/
	JsonNode parse(const std::vector<JsonToken>& tokens, int index, int& newIndex);


	class JsonConvert
	{
	public:
		static JsonNode Parse(const std::string& text);
		static std::string Serialize(const JsonNode& node);
	};

	struct JsonFormatOption
	{
		bool		Inline;				// Whether we should indent and make new-line when necessary
		bool		UseTab;				// True if we use tab to indent
		bool		KeysWithQuotes;		// True if we want keys to have quotes
	};

	// using JsonValue = void*;
	class JsonNode
	{
	public:
		JsonNode();
		template<typename T>
		JsonNode(T value, t_enable_if_same_type<T, bool> = nullptr);
		template<typename T>
		JsonNode(T value, t_enable_if_integral_type<T> = nullptr);
		template<typename T>
		JsonNode(T value, t_enable_if_floating_type<T> = nullptr);
		JsonNode(const char* value);
		JsonNode(const std::string& value);
		JsonNode(array_type_init list);
		JsonNode(object_type_init list);

		~JsonNode();

		template<typename T>
		T Get() const;

		ValueType GetType() const { return m_type; }
		std::string ToString(const JsonFormatOption& format) const;

		void foreach(std::function<void(const JsonNode&)> action) const;

		void push_back(JsonNode&& node);
		void push_back(const JsonNode& node);

		JsonNode& operator[](const std::string& name);
		const JsonNode& operator[](const std::string& name) const;
		JsonNode& operator[](size_t index);
		const JsonNode& operator[](size_t index) const;
	private:
		ValueType m_type;
		JsonValue m_value;

		std::string internal_tostring(const JsonFormatOption& format, int level) const;
	};


	JsonNode::~JsonNode()
	{
	}

	JsonNode::JsonNode()
		: m_type(ValueType::Null), m_value(std::monostate())
	{
	}


	template<typename T>
	inline JsonNode::JsonNode(T value, t_enable_if_same_type<T, bool>)
		: m_type(ValueType::Boolean), m_value(static_cast<bool>(value))
	{
	}

	template<typename T>
	inline JsonNode::JsonNode(T value, t_enable_if_integral_type<T>)
		: m_type(ValueType::Integer), m_value(static_cast<int64_t>(value))
	{
	}

	template<typename T>
	inline JsonNode::JsonNode(T value, t_enable_if_floating_type<T>)
		: m_type(ValueType::Float), m_value(static_cast<double>(value))
	{
	}


	inline JsonNode::JsonNode(const char* value)
		: m_type(ValueType::String), m_value(std::string(value))
	{
	}

	JsonNode::JsonNode(const std::string& value)
		: m_type(ValueType::String), m_value(value)
	{
	}

	inline JsonNode::JsonNode(array_type_init list)
		: m_type(ValueType::Array), m_value(array_type(list))
	{
	}

	inline JsonNode::JsonNode(object_type_init list)
		: m_type(ValueType::Object), m_value(object_type(list))
	{
	}

	template<typename T>
	inline T JsonNode::Get() const
	{
		static_assert(false, "Cannot get an un-supportted type");
	}

	template<>
	inline int JsonNode::Get() const
	{
		assert(m_type == ValueType::Integer);
		return std::get<int64_t>(m_value);
	}

	template<>
	inline int64_t JsonNode::Get() const
	{
		assert(m_type == ValueType::Integer);
		return std::get<int64_t>(m_value);
	}

	template<>
	inline float JsonNode::Get() const
	{
		assert(m_type == ValueType::Float);
		return std::get<double>(m_value);
	}

	template<>
	inline double JsonNode::Get() const
	{
		assert(m_type == ValueType::Float);
		return std::get<double>(m_value);
	}

	template<>
	inline bool JsonNode::Get() const
	{
		assert(m_type == ValueType::Boolean);
		return std::get<bool>(m_value);
	}

	template<>
	inline std::string JsonNode::Get() const
	{
		assert(m_type == ValueType::String);
		return std::get<std::string>(m_value);
	}

	std::string JsonNode::ToString(const JsonFormatOption& format) const
	{
		return internal_tostring(format, 0);
	}

	inline void JsonNode::foreach(std::function<void(const JsonNode&)> action) const
	{
		for (auto& element : std::get<array_type>(m_value))
		{
			action(element);
		}
	}

	inline void JsonNode::push_back(JsonNode&& node)
	{
		std::get<array_type>(m_value).push_back(std::move(node));
	}

	inline void JsonNode::push_back(const JsonNode& node)
	{
		std::get<array_type>(m_value).push_back(node);
	}

	inline JsonNode& JsonNode::operator[](const std::string& name)
	{
		if (m_type == ValueType::Null)
		{
			m_type = ValueType::Object;
			m_value = object_type();
		}
		return std::get<object_type>(m_value)[name];
	}

	inline const JsonNode& JsonNode::operator[](const std::string& name) const
	{
		auto& map = std::get<object_type>(m_value);
		auto it = map.find(name);
		if (it == map.end())
		{
			throw std::logic_error("Given key does not exist");
		}
		return it->second;
	}

	inline JsonNode& JsonNode::operator[](size_t index)
	{
		return std::get<array_type>(m_value)[index];
	}

	inline const JsonNode& JsonNode::operator[](size_t index) const
	{
		return std::get<array_type>(m_value)[index];
	}


	inline std::string JsonNode::internal_tostring(const JsonFormatOption& format, int level) const
	{
		auto indent = [&](std::string& str, int L) {
			if (!format.Inline)
			{
				for (int i = 0; i < L + 1; i++)
				{
					if (format.UseTab)
					{
						str.push_back('\t');
					}
					else
					{
						str.append("  ");
					}
				}
			}
		};
		switch (m_type)
		{
		case SJson::ValueType::Null:
			return "null";
		case SJson::ValueType::Object:
		{
			std::string objList = "{";
			if (!format.Inline)
			{
				objList.push_back('\n');
			}
			auto& dict = std::get<object_type>(m_value);
			int count = dict.size();
			for (auto& node : dict)
			{
				indent(objList, level);
				if (format.KeysWithQuotes)
				{
					objList.push_back('\"');
					objList.append(node.first);
					objList.push_back('\"');
				}
				else
				{
					objList.append(node.first);
				}
				objList.append(": ");
				objList.append(node.second.internal_tostring(format, level + 1));
				if (--count > 0)
				{
					objList.append(", ");
				}
				if (!format.Inline)
				{
					objList.push_back('\n');
				}
			}
			indent(objList, level - 1);
			objList.push_back('}');
			return objList;
		}
		break;
		case SJson::ValueType::Array:
		{
			std::string arrayStr = "[";
			if (!format.Inline)
			{
				arrayStr.push_back('\n');
			}
			auto& dict = std::get<array_type>(m_value);
			int count = dict.size();
			for (auto& node : dict)
			{
				indent(arrayStr, level);
				arrayStr.append(node.internal_tostring(format, level + 1));
				if (--count > 0)
				{
					arrayStr.append(", ");
				}
				if (!format.Inline)
				{
					arrayStr.push_back('\n');
				}
			}
			indent(arrayStr, level - 1);
			arrayStr.push_back(']');
			return arrayStr;
		}
		break;
		case SJson::ValueType::String:
		{
			std::string arrayStr = "\"";
			auto strValue = std::get<std::string>(m_value);
			arrayStr.append(strValue);
			arrayStr.push_back('\"');
			return arrayStr;
		}
		break;
		case SJson::ValueType::Boolean:
		{
			auto boolValue = std::get<bool>(m_value);
			return boolValue ? "true" : "false";
		}
		break;
		case SJson::ValueType::Integer:
		{
			auto intValue = std::get<int64_t>(m_value);
			return std::to_string(intValue);
		}
		break;
		case SJson::ValueType::Float:
		{
			auto floatValue = std::get<double>(m_value);
			return std::to_string(floatValue);
		}
		break;
		default:
			break;
		}
		return "";
	}

	struct JsonToken
	{
		std::string						Value;
		TokenType						Token;
		int								Position;
		std::shared_ptr<std::string>	OriginalText;
	};

	/**
	 * @brief Get the formatted std::string
	 * @tparam ...Args
	 * @param format The same format as printf
	 * @param ...args The same arguments as arguments of printf
	 * @return Formatted string
	*/
	template<typename ... Args>
	inline std::string string_format(const std::string& format, Args&& ... args)
	{
		int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		if (size_s <= 0) { throw std::exception("Error during formatting."); }
		auto size = static_cast<size_t>(size_s);
		auto buf = std::make_unique<char[]>(size);
		std::snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}


	class parse_error : public std::exception
	{
	public:
		explicit parse_error(const std::string& reason, const JsonToken& token)
		{
			int left = std::max(token.Position - 10, 0);
			int right = std::min(token.Position + 10, (int)token.OriginalText->size() - 1);
			std::string view = token.OriginalText->substr(left, right - left + 1);
			std::string viewptr = "";
			int offset = token.Position - left;
			for (int i = 0; i < offset; i++)
			{
				viewptr.push_back(' ');
			}
			viewptr.push_back('^');
			m_msg = string_format("Error: %s\nAt: %d\n%s\n%s", reason.c_str(), token.Position, view.c_str(), viewptr.c_str());
		}
		~parse_error() override {}

		const char* what() const override
		{
			return m_msg.c_str();
		}
	private:
		std::string m_msg;
	};

	inline bool is_white_space(const char c)
	{
		return c == ' ' || c == '\t' || c == '\n' || c == '\r';
	}

	inline int try_lex_keyword(const std::string& text, int index, const std::string& word)
	{
		auto len = text.size();
		auto lenWord = word.size();
		for (int i = 0; i < lenWord; i++)
		{
			if (index + i >= len || text[index + i] != word[i]) return -1;
		}
		return index + lenWord - 1;
	}

	inline int lex_remove_whole_line(const std::string& text, int index)
	{
		auto len = text.size();
		int i = index;
		while (i < len && text[i] != '\n')
		{
			i++;
		}
		return i;
	}

	inline int try_lex_string(const std::string& text, int index)
	{
		auto len = text.size();
		int i = index + 1;
		while (i < len && text[i] != '\"')
		{
			i++;
		}
		if (i == len)
		{
			//throw parse_error("Unexpected EOF while parsing string", JsonToken{ "", TokenType::String, i, });
			return -1;
		}
		return i;
	}


	struct DFAState
	{
		std::string					Name;
		std::function<int(char c)>	T;
		bool						Accept;
	};

	inline int try_lex_number(const std::string& text, int index, bool& integer)
	{
		static std::vector<DFAState> dfa = {
			DFAState{ "Init",[](char c)
			{
				if (isdigit(c)) return 2;
				else if (c == '+' || c == '-') return 1;
				else if (c == '.') return 3;
				return -1;
			}, false},
			DFAState{ "Num 0",[](char c)
			{
				if (isdigit(c)) return 2;
				else if (c == '.') return 3;
				return -1;
			}, false},
			DFAState{ "Num 1",[](char c)
			{
				if (isdigit(c)) return 2;
				else if (c == '.') return 4;
				else if (c == 'e' || c == 'E') return 5;
				return -1;
			}, true},
			DFAState{ "Dot 0",[](char c)
			{
				if (isdigit(c)) return 4;
				return -1;
			}, false},
			DFAState{ "Dot 1",[](char c)
			{
				if (isdigit(c)) return 4;
				if (c == 'e' || c == 'E') return 5;
				return -1;
			}, true},
			DFAState{ "Exp 0",[](char c)
			{
				if (isdigit(c)) return 7;
				if (c == '+' || c == '-') return 6;
				return -1;
			}, false},
			DFAState{ "Exp 0.5",[](char c)
			{
				if (isdigit(c)) return 7;
				return -1;
			}, false},
			DFAState{ "Exp 1",[](char c)
			{
				if (isdigit(c)) return 7;
				return -1;
			}, true},
		};

		auto len = text.size();
		int dfaPtr = 0;
		int i = index;
		integer = true;
		for (; i < len; i++)
		{
			auto& curState = dfa[dfaPtr];
			int nxt = curState.T(text[i]);
			if (nxt == -1)
			{
				if (curState.Accept)
				{
					return i - 1;
				}
				else
				{
					return -1;
				}
			}
			if (nxt == 4 || nxt == 7)
			{
				integer = false;
			}
			dfaPtr = nxt;
		}
		if (!dfa[dfaPtr].Accept) return -1;
		return i - 1;


		//char c = text[index];
		//int i = index;
		//integer = true;

		//bool valueBeforeDot = false;
		//bool valueAfterDot = false;

		//int state = 0;
		//if (c != '+' && c != '-' && c != '.' && !isdigit(c)) return -1;
		//
		//if (!isdigit(c))
		//{
		//	i++;
		//}
		//if (c == '.')
		//{
		//	state = 1;
		//	integer = false;
		//}

		//for (; i < len; i++)
		//{
		//	char ch = text[i];
		//	if (is_white_space(ch))
		//	{
		//		break;
		//	}
		//	if (state == 0)
		//	{
		//		if (isdigit(ch))
		//		{
		//			valueBeforeDot = true;
		//		}
		//		if (!isdigit(ch) && ch != '.')
		//		{
		//			break;
		//		}
		//		else if (ch == '.')
		//		{
		//			state = 1;
		//			integer = true;
		//		}
		//	}
		//	else if (state == 1)
		//	{
		//		if (isdigit(ch))
		//		{
		//			valueAfterDot = true;
		//		}
		//		if (!isdigit(ch))
		//		{
		//			break;
		//		}
		//	}
		//}
		//if (!valueBeforeDot && !valueAfterDot) return -1;
		return i - 1;
	}

	inline int try_lex(const std::string& text, int index, JsonToken& token)
	{
		int c = text[index];
		int newIndex = -1;
		token.Position = index;
		switch (c)
		{
		case 'n':
		{
			if ((newIndex = try_lex_keyword(text, index, "null")) != -1)
			{
				token.Value = "";
				token.Token = TokenType::Null;
			}
		}
		break;
		case 't':
		{
			if ((newIndex = try_lex_keyword(text, index, "true")) != -1)
			{
				token.Value = "";
				token.Token = TokenType::True;
			}
		}
		break;
		case 'f':
		{
			if ((newIndex = try_lex_keyword(text, index, "false")) != -1)
			{
				token.Value = "";
				token.Token = TokenType::False;
			}
		}
		break;
		case '{':
		{
			newIndex = index;
			token.Value = c;
			token.Token = TokenType::LeftBrace;
		}
		break;
		case '}':
		{
			newIndex = index;
			token.Value = c;
			token.Token = TokenType::RightBrace;
		}
		break;
		case '[':
		{
			newIndex = index;
			token.Value = c;
			token.Token = TokenType::LeftBracket;
		}
		break;
		case ']':
		{
			newIndex = index;
			token.Value = c;
			token.Token = TokenType::RightBracket;
		}
		break;
		case ',':
		{
			newIndex = index;
			token.Value = c;
			token.Token = TokenType::Comma;
		}
		break;
		case ':':
		{
			newIndex = index;
			token.Value = c;
			token.Token = TokenType::Colon;
		}
		break;
		case '#':
		{
			newIndex = lex_remove_whole_line(text, index);
			token.Value = c;
			token.Token = TokenType::Comment;
		}
		break;
		case '\"':
		{
			if ((newIndex = try_lex_string(text, index)) != -1)
			{
				token.Value = text.substr(index + 1, newIndex - index - 1);
				token.Token = TokenType::String;
			}
		}
		break;
		default:
		{
			bool integer;
			if ((newIndex = try_lex_number(text, index, integer)) != -1)
			{
				token.Value = text.substr(index, newIndex - index + 1);
				token.Token = integer ? TokenType::Integer : TokenType::Float;
			}
		}
		break;
		}
		return newIndex;
	}

	inline std::vector<JsonToken> lex(const std::string& text)
	{
		std::vector<JsonToken> tokenList;
		auto originalText = std::make_shared<std::string>(text);
		int length = text.size();
		for (int i = 0; i < length; i++)
		{
			if (is_white_space(text[i]))
			{
				continue;
			}
			if (i == length) break;
			JsonToken token;
			int newIndex = try_lex(text, i, token);
			if (newIndex != -1)
			{
				i = newIndex;
				if (token.Token == TokenType::Comment)
				{
					continue;
				}
				else
				{
					token.OriginalText = originalText;
					tokenList.push_back(token);
				}
			}
			else
			{
				throw parse_error("Invalid Token", JsonToken{ "", TokenType::Unknown, i, originalText });
			}
		}
		tokenList.push_back(JsonToken{ "", TokenType::EndOfFile, length, originalText });
		return tokenList;
	}

	int expect(const JsonToken& token, TokenType type, int index)
	{
		if (token.Token != type)
		{
			throw parse_error("Expected token #", token);
		}
		return index + 1;
	}

	using maplist = std::vector<std::pair<const std::string, JsonNode>>;
	using m_pair = std::pair<const std::string, JsonNode>;
	using arrlist = std::vector<JsonNode>;

	maplist parse_object(const std::vector<JsonToken>& tokens,
		int index, int& newIndex)
	{

		maplist list;
		int curIndex = index;
		if (tokens[curIndex].Token != TokenType::RightBrace)
		{
			while (true)
			{
				auto& keyToken = tokens[curIndex++];
				if (keyToken.Token != TokenType::String)
				{
					throw parse_error("Expected keys to be string", keyToken);
				}
				const std::string key = keyToken.Value;

				curIndex = expect(tokens[curIndex], TokenType::Colon, curIndex);
				int nxtIndex;
				auto value = parse(tokens, curIndex, nxtIndex);
				curIndex = nxtIndex;

				list.push_back(m_pair{ key, value });

				if (tokens[curIndex].Token == TokenType::Comma)
				{
					curIndex++;
				}
				else
				{
					break;
				}
			}
			newIndex = expect(tokens[curIndex], TokenType::RightBrace, curIndex);
			return list;
		}
		else
		{
			newIndex = index + 1;
			return list;
		}
	}

	arrlist parse_array(const std::vector<JsonToken>& tokens,
		int index, int& newIndex)
	{
		arrlist list;
		int curIndex = index;
		if (tokens[curIndex].Token != TokenType::RightBracket)
		{
			while (true)
			{
				int nxtIndex;
				list.push_back(parse(tokens, curIndex, nxtIndex));
				curIndex = nxtIndex;
				if (tokens[curIndex].Token == TokenType::Comma)
				{
					curIndex++;
				}
				else
				{
					break;
				}
			}
			newIndex = expect(tokens[curIndex], TokenType::RightBracket, curIndex);
			return list;
		}
		else
		{
			newIndex = index + 1;
			return list;
		}
	}

	inline std::string remove_escapes(const std::string& str, const JsonToken& token)
	{
		std::string result;
		int len = str.size();
		for (int i = 0; i < len; i++)
		{
			if (str[i] != '\\')
			{
				result.push_back(str[i]);
				continue;
			}
			if (str[i] == '\\')
			{
				if (i == len - 1)
				{
					throw parse_error("Invalid escape character", token);
				}
				switch (str[i + 1])
				{
				case 'n':
					result.push_back('\n');
					i++;
					break;
				case 'r':
					result.push_back('\r');
					i++;
					break;
				case 't':
					result.push_back('\t');
					i++;
					break;
				case '\\':
					result.push_back('\\');
					i++;
					break;
				default:
					throw parse_error("Invalid escape character", token);
					break;
				}
			}
		}
		return result;
	}

	JsonNode parse(const std::vector<JsonToken>& tokens, int index, int& newIndex)
	{
		auto& token = tokens[index];
		switch (token.Token)
		{
		case TokenType::Null:
		{
			newIndex = index + 1;
			return JsonNode();
		}
		case TokenType::Integer:
		{
			newIndex = index + 1;
			size_t num;
			return JsonNode(std::stoll(token.Value, &num, 10));
		}
		case TokenType::Float:
		{
			newIndex = index + 1;
			size_t num;
			return JsonNode(std::stod(token.Value, &num));
		}
		case TokenType::True:
		{
			newIndex = index + 1;
			return JsonNode(true);
		}
		case TokenType::False:
		{
			newIndex = index + 1;
			return JsonNode(false);
		}
		case TokenType::String:
		{
			newIndex = index + 1;
			return JsonNode(remove_escapes(token.Value, token));
		}
		case TokenType::LeftBrace:
		{
			JsonNode node = JsonNode(object_type_init());
			for (auto& pair : parse_object(tokens, index + 1, newIndex))
			{
				node[pair.first] = pair.second;
			}
			return node;
		}
		case TokenType::LeftBracket:
		{
			JsonNode node = JsonNode(array_type_init());
			for (auto& element : parse_array(tokens, index + 1, newIndex))
			{
				node.push_back(element);
			}
			return node;
		}
		case TokenType::EndOfFile:
		{
			throw parse_error("Unexpected EOF!", token);
		}
		default:
			break;
		}
		throw parse_error("Failed to parse", token);
	}

	JsonNode JsonConvert::Parse(const std::string& text)
	{
		auto tokens = lex(text);
		int index = 0;
		auto node = parse(tokens, 0, index);
		if (tokens[index].Token != TokenType::EndOfFile)
		{
			throw parse_error("Root not singluar", tokens[index]);
		}
		return node;
	}

	std::string JsonConvert::Serialize(const JsonNode& node)
	{
		return std::string();
	}

	inline std::string GetValueTypeName(ValueType type)
	{
		switch (type)
		{
		case SJson::ValueType::Null:
			return "Null";
		case SJson::ValueType::Object:
			return "Object";
		case SJson::ValueType::Array:
			return "Array";
		case SJson::ValueType::String:
			return "String";
		case SJson::ValueType::Boolean:
			return "Boolean";
		case SJson::ValueType::Integer:
			return "Integer";
		case SJson::ValueType::Float:
			return "Float";
		case SJson::ValueType::__COUNT:
		default:
			break;
		}
		return "ERROR TYPE";
	}

	inline JsonNode object(std::initializer_list<std::pair<const std::string, JsonNode>> list)
	{
		return JsonNode(list);
	}

	inline JsonNode array(std::initializer_list<JsonNode> list)
	{
		return JsonNode(list);
	}

	//std::string SJsonGetNodeTypeName(SJsonNodeType type);

	//class ParseError : public std::exception
	//{
	//public:
	//	explicit ParseError(const char* name, int line, int col) : std::exception(name), _line(line), _col(col)
	//	{
	//	}
	//	int GetLine() const { return _line; }
	//	int GetCol() const { return _col; }
	//private:
	//	int _line, _col;
	//};

	//class RootNotSingularError : public ParseError
	//{
	//public:
	//	explicit RootNotSingularError(int line, int col) : ParseError("RootNotSingularError", line, col) {}
	//};

	//class ExpectValueError : public ParseError
	//{
	//public:
	//	explicit ExpectValueError(int line, int col) : ParseError("ExpectValueError", line, col) {}
	//};

	//class InvalidValueError : public ParseError
	//{
	//public:
	//	explicit InvalidValueError(int line, int col) : ParseError("InvalidValueError", line, col) {}
	//};

	//class UnexpectedTokenError : public ParseError
	//{
	//public:
	//	explicit UnexpectedTokenError(int line, int col, const std::string& c) : ParseError("UnexpectedTokenError", line, col), token(c) {}
	//	std::string token;
	//};

	//class ExpectTokenError : public ParseError
	//{
	//public:
	//	explicit ExpectTokenError(int line, int col, const std::string& c, const std::string& cur) : ParseError("ExpectTokenError", line, col), token(c), curToken(cur) {}
	//	std::string token, curToken;
	//};

	//class OperationError : public std::exception
	//{
	//public:
	//	explicit OperationError(const char* name) : std::exception(name) {}
	//};

	//class ConversionError : public OperationError
	//{
	//public:
	//	explicit ConversionError(const std::string& fr, const std::string& to) : OperationError("Conversion Error"),
	//		from(fr), to(to)
	//	{
	//	}
	//	std::string from, to;
	//};

	//class MissingMemberError : public OperationError
	//{
	//public:
	//	explicit MissingMemberError(const std::string& s) : OperationError("Member Not Found Error"),
	//		member(s)
	//	{
	//	}
	//	std::string member;
	//};


	///// <summary>
	///// SJsonNode 是Json节点的基类，包含各种取值判断操作
	///// </summary>
	//class SJsonNode
	//{
	//public:
	//	SJsonNode() = default;
	//	virtual ~SJsonNode() = 0 {}

	//	virtual SJsonNodeType GetType() const = 0;

	//	virtual bool IsNull() const { return false; }

	//	virtual ll GetInt() const
	//	{
	//		throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_INT));
	//	}
	//	virtual double GetFloat() const
	//	{
	//		throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_FLOAT));
	//	}
	//	virtual bool GetBool() const
	//	{
	//		throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_BOOL));
	//	}
	//	virtual std::string GetString() const
	//	{
	//		throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_STRING));
	//	}

	//	// Array 操作
	//	virtual void ForEachElements(std::function<void(const SJsonNode*)> func) const
	//	{
	//		throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_ARRAY));
	//	}
	//	virtual std::size_t arraySize() const
	//	{
	//		throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_ARRAY));
	//	}
	//	virtual const SJsonNode* ElementAt(int index) const
	//	{
	//		throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_ARRAY));
	//	}

	//	// Object 操作
	//	virtual const SJsonNode* GetMember(const std::string& name) const
	//	{
	//		throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_OBJECT));
	//	}

	//	virtual bool HasMember(const std::string& name) const
	//	{
	//		throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_OBJECT));
	//	}


	//	virtual void ForEachProperties(std::function<void(const std::string&, const SJsonNode*)> func) const
	//	{
	//		throw ConversionError(SJsonGetNodeTypeName(GetType()), SJsonGetNodeTypeName(SJsonNodeType::JSON_OBJECT));
	//	}
	//};


	//class SJsonObjectNode : public SJsonNode
	//{
	//public:
	//	SJsonObjectNode(const std::map<std::string, std::shared_ptr<SJsonNode>>& items) : _value(items) {}
	//	~SJsonObjectNode() override {}

	//	SJsonNodeType GetType() const override { return SJsonNodeType::JSON_OBJECT; };
	//	const SJsonNode* GetMember(const std::string& name) const override
	//	{
	//		auto v = _value.find(name);
	//		if (v == _value.end()) throw MissingMemberError(name);
	//		return ptr(v->second);
	//	}
	//	bool HasMember(const std::string& name) const override
	//	{
	//		return _value.find(name) != _value.end();
	//	}

	//	void ForEachProperties(std::function<void(const std::string&, const SJsonNode*)> func) const override
	//	{
	//		for (auto& node : _value)
	//		{
	//			func(node.first, cptr(node.second));
	//		}
	//	}
	//private:
	//	std::map<std::string, std::shared_ptr<SJsonNode>> _value;
	//};


	//class SJsonArrayNode : public SJsonNode
	//{
	//public:
	//	SJsonArrayNode(const std::vector<std::shared_ptr<SJsonNode>>& items) : _values(items) {}
	//	~SJsonArrayNode() override {}

	//	SJsonNodeType GetType() const override { return SJsonNodeType::JSON_ARRAY; };

	//	std::size_t arraySize() const override { return _values.size(); }
	//	void ForEachElements(std::function<void(const SJsonNode*)> func) const override
	//	{
	//		for (auto& v : _values)
	//		{
	//			func(cptr(v));
	//		}
	//	}
	//	const SJsonNode* ElementAt(int index) const override
	//	{
	//		return cptr(_values[index]);
	//	}
	//private:
	//	std::vector<std::shared_ptr<SJsonNode>> _values;
	//};

	//class SJsonNullNode : public SJsonNode
	//{
	//public:
	//	SJsonNullNode() = default;
	//	~SJsonNullNode() override {}

	//	SJsonNodeType GetType() const override { return SJsonNodeType::JSON_NULL; };
	//	bool IsNull() const override { return true; }
	//};

	//class SJsonBoolNode : public SJsonNode
	//{
	//public:
	//	SJsonBoolNode(bool value) : SJsonNode(), _value(value) {}
	//	~SJsonBoolNode() override {}

	//	SJsonNodeType GetType() const override { return SJsonNodeType::JSON_BOOL; };
	//	bool GetBool() const override { return _value; }

	//private:
	//	bool _value;
	//};


	//class SJsonIntNode : public SJsonNode
	//{
	//public:
	//	SJsonIntNode(ll value) : SJsonNode(), _value(value) {}
	//	~SJsonIntNode() override {}

	//	SJsonNodeType GetType() const override { return SJsonNodeType::JSON_INT; };
	//	ll GetInt() const override { return _value; }
	//	double GetFloat() const override { return (double)_value; }

	//private:
	//	ll _value;
	//};

	//class SJsonFloatNode : public SJsonNode
	//{
	//public:
	//	SJsonFloatNode(double value) : SJsonNode(), _value(value) {}
	//	~SJsonFloatNode() override {}

	//	SJsonNodeType GetType() const override { return SJsonNodeType::JSON_FLOAT; };
	//	double GetFloat() const override { return _value; }
	//	ll GetInt() const override { return (ll)_value; }

	//private:
	//	double _value;
	//};

	//class SJsonStringNode : public SJsonNode
	//{
	//public:
	//	SJsonStringNode(const std::string& value) : SJsonNode(), _value(value) {}
	//	~SJsonStringNode() override {}

	//	SJsonNodeType GetType() const override { return SJsonNodeType::JSON_STRING; };
	//	std::string GetString() const override { return _value; }

	//private:
	//	std::string _value;
	//};




	//// 模板便捷操作
	//template<typename T>
	//T TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, T defValue);

	//template<typename T>
	//inline T TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, T defValue)
	//{
	//	throw OperationError("Unknown JSON Value Type");
	//}
	//template<>
	//inline int TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, int defValue)
	//{
	//	if (node->HasMember(name))
	//	{
	//		return node->GetMember(name)->GetInt();
	//	}
	//	return defValue;
	//}
	//template<>
	//inline ll TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, ll defValue)
	//{
	//	if (node->HasMember(name))
	//	{
	//		return node->GetMember(name)->GetInt();
	//	}
	//	return defValue;
	//}

	//template<>
	//inline bool TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, bool defValue)
	//{
	//	if (node->HasMember(name))
	//	{
	//		return node->GetMember(name)->GetBool();
	//	}
	//	return defValue;
	//}

	//template<>
	//inline float TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, float defValue)
	//{
	//	if (node->HasMember(name))
	//	{
	//		return node->GetMember(name)->GetFloat();
	//	}
	//	return defValue;
	//}

	//template<>
	//inline double TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, double defValue)
	//{
	//	if (node->HasMember(name))
	//	{
	//		return node->GetMember(name)->GetFloat();
	//	}
	//	return defValue;
	//}

	//template<>
	//inline std::string TryGetMemberValue(const std::shared_ptr<SJsonNode>& node, const std::string& name, std::string defValue)
	//{
	//	if (node->HasMember(name))
	//	{
	//		return node->GetMember(name)->GetString();
	//	}
	//	return defValue;
	//}
}
