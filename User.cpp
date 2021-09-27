#include "User.hpp"

User::User(int sockfd) :
sockfd(sockfd), role(client)
{
	(void)role;
}

User::~User()
{}

const int					&User::getSockfd() const
{
	return sockfd;
}

void						User::readMessage()
{
	std::string	text;
	if (messages.size() > 0)
		text = messages.front();
	char buffer[100];
	int bytesRead;
	while ((bytesRead = read(sockfd, buffer, 99)) > 0)
	{
		buffer[bytesRead] = 0;
		text += buffer;
		buffer[0] = 0;
		if (text.find('\n'))
			break;
	}
	messages = split(text, '\n', true);
}

void						logMessage(const Message &msg)
{
	std::cout << "prefix = " << msg.getPrefix() << ", command = " << msg.getCommand();
	std::cout << ", paramsCount = " << msg.getParams().size() << std::endl;
	auto beg = msg.getParams().begin();
	auto end = msg.getParams().end();
	while (beg != end)
	{
		if (beg == msg.getParams().begin())
			std::cout << "Params list: \"" << *beg << "\"";
		else
			std::cout << ", \"" << *beg << "\"";
		++beg;
		if (beg == end)
			std::cout << std::endl;
	}
}

int							User::hadleMessages()
{
	while (messages.size() > 0 && messages.front().back() == '\n')
	{
		Message	msg(messages.front());
		messages.pop();
		// log message to server console
		logMessage(msg);
		// handle
		if (msg.getCommand() == "end")
		{
			std::string response = "Good talking to you\n";
			send(sockfd, response.c_str(), response.size(), 0);
			// Close the connections
			close(sockfd);
			return (-1);
		}
		else if (msg.getCommand() == "help")
			send(sockfd, "Ne pomogu!\n", 11, 0);
		else if (msg.getCommand() == "HELP")
			send(sockfd, "ПОЧЕМУ \"Г\" ПЕРЕВЁРНУТАЯ?!\n", 45, 0);
	}
	return (0);
}
