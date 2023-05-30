#ifndef COMMCHANNEL_H
#define COMMCHANNEL_H

#include <optional>
#include <queue>
#include <mutex>
#include <iostream>

template<typename T>
class CommChannel {

public:
	using value_type = T;
	std::optional<value_type> receive() {
		std::unique_lock<std::mutex> lck(mtx);
		cv.wait(lck, [this]() {return (!comm_channel_.empty() || closed_);});
		if (closed_) 
		{
			return std::nullopt;;
		}
		T value = std::move(comm_channel_.back());
		comm_channel_.pop_back();

		return value;
	}

	void send(value_type &&value) {
		std::lock_guard<std::mutex> lck(mtx);
		std::cout << value << std::endl;
		comm_channel_.push_back(std::move(value));
		cv.notify_one();
	}

    void close() {
        std::lock_guard<std::mutex> locker(mtx);
        closed_ = true;
        cv.notify_all();
    }

    

private:
	std::mutex mtx;
	std::condition_variable cv;
	std::deque<value_type> comm_channel_;
	bool closed_ = false;  
};


#endif

