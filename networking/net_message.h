#pragma once
#include "net_common.h"

namespace olc 
{
	namespace net 
	{
		// Message header is sent at start of all messages. The template allows us
		// to use "enum class" to ensure that the messages are valid at compile time
		template <typename T>
		struct message_header 
		{
			T id{};
			uint32_t size = 0;
		};
		
		template <typename T>
		struct message
		{
			message_header<T> header;
			std::vector<uint8_t> body;

			// returns the size of the entire message packet in bytes
			size_t size() const {
				return body.size();
			}

			// override for std::cout compatibility - produces friendly description of message
			friend std::ostream& operator << (std::ostream& os, const message<T>& msg) {
				os << "ID: " << int(msg.header.id) << " Size: " << msg.header.size();
				return os;
			}

			// pushes any POD-like data into the message buffer
			template<typename DataType>
			friend message<T>& operator << (message<T>& msg, const DataType& data) {
				// check that the type of the data being pushed is trivially copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into a vector");

				// cache the current size of the vector, as this will be the point we insert the data
				size_t i = msg.body.size();

				// resize the vector by the size of the data being pushed
				msg.body.resize(msg.body.size() + sizeof(DataType));

				// physically copy the data into the newly allocated vector space
				std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

				// recalculate message size
				msg.header.size = msg.size();

				// Return the target message so it can be "chained"
				return msg;
			}

			// Pulls any POD-like data from the message buffer
			template<typename DataType>
			friend message<T>& operator >> (message<T>& msg, DataType& data)
			{
				// Check that the type of the data being pushed is trivially copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

				// Cache the location towards the end of the vector where the pulled data starts
				size_t i = msg.body.size() - sizeof(DataType);

				// Physically copy the data from the vector into the user variable
				std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

				// Shrink the vector to remove read bytes, and reset end position
				msg.body.resize(i);

				// Recalculate the message size
				msg.header.size = msg.size();

				// Return the target message so it can be "chained"
				return msg;
			}

			// Pushes vector data into the message buffer
			friend message<T>& operator << (message<T>& msg, const std::vector<int>& data) {
				// check that the type of the data being pushed is trivially copyable
				//static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into a vector");

				// cache the current size of the vector, as this will be the point we insert the data
				size_t i = msg.body.size();

				// resize the vector by the size of the data being pushed
				msg.body.resize(msg.body.size() + data.size() * sizeof(int));

				// physically copy the data into the newly allocated vector space
				// since we're pushing a vector it needs to be done one by one
				for (int j = 0; j < data.size(); j++)
				{
					std::memcpy(msg.body.data() + i, &data.at(j), sizeof(int));
					i += sizeof(int);
				}

				// recalculate message size
				msg.header.size = msg.size();

				// Return the target message so it can be "chained"
				return msg;
			}

			// Pulls vector data from the message buffer
			friend message<T>& operator >> (message<T>& msg, std::vector<int>& data)
			{
				// Check that the type of the data being pushed is trivially copyable
				//static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

				// Cache the location towards the end of the vector where the pulled data starts
				size_t i = msg.body.size() - data.size() * sizeof(int);

				// Physically copy the data from the vector into the user variable
				// since we're pulling from a vector it needs to be done one by one
				for (int j = 0; j < data.size(); j++)
				{
					std::memcpy(&data.at(j), msg.body.data() + i, sizeof(int));
					i += sizeof(int);
				}

				i = i - data.size() * sizeof(int);
			
				// Shrink the vector to remove read bytes, and reset end position
				msg.body.resize(i);

				// Recalculate the message size
				msg.header.size = msg.size();

				// Return the target message so it can be "chained"
				return msg;
			}
		};

		// forward declare the connection
		template<typename T>
		class Connection;

		template<typename T>
		struct owned_message
		{
			std::shared_ptr<Connection<T>> remote = nullptr;
			message<T> msg;

			// again, a friendly string maker
			friend std::ostream& operator << (std::ostream& os, const owned_message<T>& msg) {
				os << msg.msg;
				return os;
			}
		};

	}
}
