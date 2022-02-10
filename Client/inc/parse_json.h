#ifndef PARSE_JSON_H_
#define PARSE_JSON_H_
#define BUF_MAX_SIZE 1000

	/** \brief Parse json file located under /tmp/access_points and 
	 *  put the extracted parameter under /tmp/CurrStats.txt.
	 * \return \c void
	 */
	void ParseJson();
	/** \brief Call func ParseJson and move old params under file
	 *  /tmp/OldStats.txt and get the new ones under /tmp/CurrStats.txt.
	 *  each 10 seconds
	 * \return \c void
	 */
	void *threadproc(void *arg);
	/** \brief Extract value of ssid, snr, channel from a given buffer
	 * \\param[out] char* Ret.
	 */
	char* GetParam(char buffer[BUF_MAX_SIZE]);
	/** \brief Compare file /tmp/OldStats.txt and /tmp/CurrStats.txt
	 *  to extract deleted Elements
	 * \return \c void
	 */
	void CheckDeletedElement();
	/** \brief Compare file /tmp/CurrStats.txt and /tmp/OldStats.txt
	 *  to extract new Elements
	 * \return \c void
	 */
	void CheckAddedElement();
	/** \brief Compare file /tmp/CurrStats.txt and /tmp/OldStats.txt
	 *  to extract modified Elements
	 * \return \c void
	 */
	void CheckModifiedElement();

#endif
