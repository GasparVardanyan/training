# include <iostream>
# include <string_view>

template <typename TImpl>
struct Notifier {
	void alert_sms (std::string_view msg) {
		impl ().send_alert_sms (msg);
	}
	void alert_email (std::string_view msg) {
		impl ().send_alert_email (msg);
	}

private:
	TImpl & impl () {
		return * static_cast <TImpl *> (this);
	}
};

struct TestNotifier : public Notifier <TestNotifier> {
	friend struct Notifier <TestNotifier>;

protected:
	void send_alert_sms (std::string_view msg) {
		std::cout << "sending sms: " << msg << std::endl;
	}
	void send_alert_email (std::string_view msg) {
		std::cout << "sending email: " << msg << std::endl;
	}
};

template <typename TImpl>
void alertAllChannels (Notifier<TImpl> & notifier, std::string_view msg) {
	notifier.alert_sms (msg);
	notifier.alert_email (msg);
}

int main ()
{
	TestNotifier tn;
	alertAllChannels (tn, "hello");
}
