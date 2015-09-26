

/**
 * @brief handles all hardware io.
 * Reads Hardware io and passes it to mqtt respectively the console
 */


class IOHandler {
public:
	IOHandler();

	/**
	 * initialize IO configuration
	 */
	void init();
	void setDiagnosticLed(bool on);

	/**
	 * handle message on hardware
	 * @param topic
	 * @param message
	 * @return true if handled
	 */
	bool handleMessage(String topic,String message);
private:

};


extern IOHandler IO;
