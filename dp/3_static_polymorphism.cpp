# include <concepts>
# include <iostream>
# include <string_view>
# include <type_traits>

template <typename T>
concept NotifierDerivedInterface = requires (T t) {
	t.sendAlertSMS (std::string_view {});
	t.sendAlertEmail (std::string_view {});
};

template <typename TImpl>
class Notifier {
public:
	Notifier () {
		static_assert (NotifierDerivedInterface <TImpl> && std::derived_from <TImpl, Notifier>);
	}
	void alertSMS (std::string_view msg) {
		impl ().sendAlertSMS (msg);
	}
	void alertEmail (std::string_view msg) {
		impl ().sendAlertEmail (msg);
	}

private:
	TImpl & impl () {
		return * static_cast <TImpl *> (this);
	}
};

class TestNotifier : public Notifier <TestNotifier> {
protected:
	friend Notifier <TestNotifier>;
	void sendAlertSMS (std::string_view msg) {
		std::cout << "N1 alert SMS: " << msg << std::endl;
	}
	void sendAlertEmail (std::string_view msg) {
		std::cout << "N2 alert Email: " << msg << std::endl;
	}
};

template <typename T, typename = std::enable_if_t <std::derived_from <T, Notifier <T>>>>
void NotifyAllChannels (Notifier <T> & notifier, std::string_view msg) {
	notifier.alertSMS (msg);
	notifier.alertEmail (msg);
}

int main () {
	TestNotifier n;
	n.alertSMS ("sms msg");
	n.alertEmail ("email msg");
	NotifyAllChannels (n, "hello, world!");
}
