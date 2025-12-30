# include <iostream>
# include <string_view>

template <typename TImpl>
concept IsANotifier = requires (TImpl impl) {
	impl.alert_sms (std::string_view {});
	impl.alert_email (std::string_view {});
};

template <IsANotifier TImpl>
void alertAllChannels (TImpl & notifier, std::string_view msg) {
	notifier.alert_sms (msg);
	notifier.alert_email (msg);
}

struct TestNotifier {
	void alert_sms (std::string_view msg) {
		std::cout << "sending sms: " << msg << std::endl;
	}
	void alert_email (std::string_view msg) {
		std::cout << "sending email: " << msg << std::endl;
	}
};

int main () {
	TestNotifier tn;
	alertAllChannels(tn, "hello");
}
