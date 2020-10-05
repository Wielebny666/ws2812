set(SOURCES ws2812.c
			)

idf_component_register(SRCS "${SOURCES}"
					INCLUDE_DIRS . )
