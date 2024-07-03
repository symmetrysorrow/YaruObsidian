import imaplib
import email
from email.header import decode_header

def get_mail_first_lines():
    gmail = "example@gmail.com" #メールを入れる場所
    password = "password" #パスワードを入れる場所

    mail = imaplib.IMAP4_SSL("imap.gmail.com")
    mail.login(gmail, password)

    
    mail.select("inbox")

    status, messages = mail.search(None, "ALL")
    mail_ids = messages[0].split()

    first_lines = []

    for mail_id in mail_ids[::-1]:
        status, msg_data = mail.fetch(mail_id, "(RFC822)")
        for response_part in msg_data:
            if isinstance(response_part, tuple):
                msg = email.message_from_bytes(response_part[1])

                if msg.is_multipart():
                    for part in msg.walk():
                        content_type = part.get_content_type()
                        content_disposition = str(part.get("Content-Disposition"))

                        if "attachment" not in content_disposition:
                            body = part.get_payload(decode=True).decode(part.get_content_charset())
                            first_line = body.split("\n", 1)[0].strip()  # 一行目を取得して空白を除去
                            if first_line:  # 空でない場合のみ追加
                                first_lines.append(first_line)
                                break
                else:
                    body = msg.get_payload(decode=True).decode(msg.get_content_charset())
                    first_line = body.split("\n", 1)[0].strip()  # 一行目を取得して空白を除去
                    if first_line:  # 空でない場合のみ追加
                        first_lines.append(first_line)

    # Delete all emails after processing
    for mail_id in mail_ids:
        mail.store(mail_id, '+FLAGS', '\\Deleted')
    mail.expunge()

    mail.logout()

    return first_lines

if __name__ == "__main__":
    lines = get_mail_first_lines()
    for line in lines:
        print(line)
