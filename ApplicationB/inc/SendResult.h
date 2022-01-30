#ifndef SEND_RESULTS_H_
#define SEND_RESULTS_H_

	/** \brief Read output from a file /tmp/Result.txt shared between 
	 *  Application A and Application B
	 * \return \c void
	 */
	void ReadData();
	/** \brief Call func ReadData each 10 seconds.
	 * \return \c void
	 */
	void *threadproc(void *arg);

#endif
