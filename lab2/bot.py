# https://t.me/BookWiseExpertBot

import logging

from telegram import ReplyKeyboardMarkup, Update
from telegram.ext import (
    Application,
    CommandHandler,
    ContextTypes,
    ConversationHandler,
    MessageHandler,
    filters,
)

from google_books_api import get_books
from config import BOT_TOKEN
from reply_keyboard import reply_keyboard
from answers_map import answers_map

logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s", level=logging.INFO
)

logging.getLogger("httpx").setLevel(logging.WARNING)

logger = logging.getLogger(__name__)

CHOOSING, TYPING_REPLY, TYPING_CHOICE = range(3)

key_value_list = list(reply_keyboard.items())

async def start(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await update.message.reply_text(
        "Привіт! Мене звати Юрій. Я твій особистий асистент у справах книг. " 
        "Будь ласка, розкажіть мені трохи про себе, "
        "щоб я міг краще вас зрозуміти та надавати корисну інформацію.",
    )

    context.user_data["q"] = ""

    question, answers = key_value_list[0]

    markup = ReplyKeyboardMarkup(answers, one_time_keyboard=True, resize_keyboard=True)

    await update.message.reply_text(
        question,
        reply_markup=markup,
    )

    return CHOOSING


async def regular_choice(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    text = update.message.text
    context.user_data["choice"] = text
    print('text: ' + text)

    if text == 'Отримати книги':
        return await get_recommended_books(update, context)

    if text == '/start':
        return await start(update, context)

    idx_new_answer = answers_map[text][0]

    context.user_data["q"] += answers_map[text][1]

    print(context.user_data["q"])
    
    question, answers = key_value_list[idx_new_answer]
   
    markup = ReplyKeyboardMarkup(answers, one_time_keyboard=True, resize_keyboard=True)

    await update.message.reply_text(
        question,
        reply_markup=markup,
    )

    return CHOOSING

async def get_recommended_books(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    book_titles = get_books(context.user_data["q"])

    for book in book_titles:
        message = f"Назва: {book['title']}\n" \
            f"Автор: {', '.join(book['authors'])}\n" \
            f"Опис: {book['description']}\n" \
            f"Дата публікації: {book['published_date']}\n"

        await update.message.reply_text(message)

    return ConversationHandler.END


def main() -> None:
    application = Application.builder().token(BOT_TOKEN).build()

    conv_handler = ConversationHandler(
        entry_points=[CommandHandler("start", start)],
        states={
            CHOOSING: [
                MessageHandler(filters.Regex("^.*$"), regular_choice),  
            ]
        },
        fallbacks=[MessageHandler(filters.Regex("^(Отримати книги)$"), get_books)]
    )


    application.add_handler(conv_handler)
    application.run_polling(allowed_updates=Update.ALL_TYPES)


if __name__ == "__main__":
    main()