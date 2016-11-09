/*
   Based on FlexBisonQt Gonzalo Exequiel Pedone Copyright (C) 2013
   Web-Site: http://github.com/hipersayanX/FlexBisonQt

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with This program.  If not, see <http://www.gnu.org/licenses/>.

   Email   : ssbayanov AT gmail DOT com

*/

%{

#include <QtGui>

extern int yylex(void);
void yyerror(const char *s);

%}

// Here we can define some custom variable types.
// The custom types must be of static size.
%union {
    QVariant *QVariant_t;
}

// Define the types for terminal expressions.
%token <QVariant_t> TOKEN_INTIGER
%token <QVariant_t> TOKEN_FLOAT
%token <QVariant_t> TOKEN_BOOLEAN
%token <QVariant_t> TOKEN_STRING

// Define the tokens for the symbols.
%token TOKEN_LEFTPAREN
%token TOKEN_RIGHTPAREN
%token TOKEN_LEFTCURLYBRACKET
%token TOKEN_RIGHTCURLYBRACKET
%token TOKEN_LEFTBRACKET
%token TOKEN_RIGHTBRACKET
%token TOKEN_COMMA
%token TOKEN_COLON

// Define the tokens for the keywords.
%token TOKEN_SIZE
%token TOKEN_SIZEF
%token TOKEN_POINT
%token TOKEN_POINTF
%token TOKEN_RECT
%token TOKEN_RECTF
%token TOKEN_LINE
%token TOKEN_LINEF
%token TOKEN_DATE
%token TOKEN_TIME
%token TOKEN_DATETIME
%token TOKEN_COLOR
%token TOKEN_BYTES
%token TOKEN_URL

// Define the types for non-terminal expressions.
%type <QVariant_t> variant
%type <QVariant_t> size
%type <QVariant_t> sizeF
%type <QVariant_t> point
%type <QVariant_t> pointF
%type <QVariant_t> rect
%type <QVariant_t> rectF
%type <QVariant_t> line
%type <QVariant_t> lineF
%type <QVariant_t> date
%type <QVariant_t> time
%type <QVariant_t> dateTime
%type <QVariant_t> color
%type <QVariant_t> bytes
%type <QVariant_t> url
%type <QVariant_t> number
%type <QVariant_t> variantListItems
%type <QVariant_t> variantList
%type <QVariant_t> variantMapPair
%type <QVariant_t> variantMapItems
%type <QVariant_t> variantMap

// Prevents memory leak in non-terminal expressions.
%destructor {delete $$;} variant
%destructor {delete $$;} size
%destructor {delete $$;} sizeF
%destructor {delete $$;} point
%destructor {delete $$;} pointF
%destructor {delete $$;} rect
%destructor {delete $$;} rectF
%destructor {delete $$;} line
%destructor {delete $$;} lineF
%destructor {delete $$;} date
%destructor {delete $$;} time
%destructor {delete $$;} dateTime
%destructor {delete $$;} color
%destructor {delete $$;} bytes
%destructor {delete $$;} url
%destructor {delete $$;} number
%destructor {delete $$;} variantListItems
%destructor {delete $$;} variantList
%destructor {delete $$;} variantMapPair
%destructor {delete $$;} variantMapItems
%destructor {delete $$;} variantMap

%%

/* Main expression to check. */
start: variant {qDebug() << *$1;};

variant: number
       | TOKEN_BOOLEAN
       | size
       | sizeF
       | point
       | pointF
       | rect
       | rectF
       | line
       | lineF
       | date
       | time
       | dateTime
       | color
       | bytes
       | url
       | TOKEN_STRING
       | variantList
       | variantMap
       ;

rect: TOKEN_RECT TOKEN_LEFTPAREN TOKEN_RIGHTPAREN {$$ = new QVariant(QRect());}
    | TOKEN_RECT TOKEN_LEFTPAREN point TOKEN_COMMA point TOKEN_RIGHTPAREN {
          // $$ is a reference to the result of the espression.
          // $1, $2, $3, ..., $N are references to each expression.
          $$ = new QVariant();
          *$$ = QRect($3->toPoint(), $5->toPoint());
      }
    | TOKEN_RECT TOKEN_LEFTPAREN point TOKEN_COMMA size TOKEN_RIGHTPAREN {
          $$ = new QVariant();
          *$$ = QRect($3->toPoint(), $5->toSize());
      }
    | TOKEN_RECT TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_COMMA number TOKEN_COMMA number TOKEN_RIGHTPAREN {
          $$ = new QVariant();
          *$$ = QRect($3->toFloat(), $5->toFloat(), $7->toFloat(), $9->toFloat());
      }
    ;

rectF: TOKEN_RECTF TOKEN_LEFTPAREN TOKEN_RIGHTPAREN {$$ = new QVariant(QRectF());}
     | TOKEN_RECTF TOKEN_LEFTPAREN pointF TOKEN_COMMA pointF TOKEN_RIGHTPAREN {
           $$ = new QVariant();
           *$$ = QRectF($3->toPointF(), $5->toPointF());
       }
     | TOKEN_RECTF TOKEN_LEFTPAREN pointF TOKEN_COMMA sizeF TOKEN_RIGHTPAREN {
           $$ = new QVariant();
           *$$ = QRectF($3->toPointF(), $5->toSizeF());
       }
     | TOKEN_RECTF TOKEN_LEFTPAREN rect TOKEN_RIGHTPAREN {
           $$ = new QVariant();
           *$$ = QRectF($3->toRect());
       }
     | TOKEN_RECTF TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_COMMA number TOKEN_COMMA number TOKEN_RIGHTPAREN {
           $$ = new QVariant();
           *$$ = QRectF($3->toFloat(), $5->toFloat(), $7->toFloat(), $9->toFloat());
       }
     ;

line: TOKEN_LINE TOKEN_LEFTPAREN TOKEN_RIGHTPAREN {$$ = new QVariant(QLine());}
    | TOKEN_LINE TOKEN_LEFTPAREN point TOKEN_COMMA point TOKEN_RIGHTPAREN {
          $$ = new QVariant();
          *$$ = QLine($3->toPoint(), $5->toPoint());
      }
    | TOKEN_LINE TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_COMMA number TOKEN_COMMA number TOKEN_RIGHTPAREN {
          $$ = new QVariant();
          *$$ = QLine($3->toFloat(), $5->toFloat(), $7->toFloat(), $9->toFloat());
      }
    ;

lineF: TOKEN_LINEF TOKEN_LEFTPAREN TOKEN_RIGHTPAREN {$$ = new QVariant(QLineF());}
     | TOKEN_LINEF TOKEN_LEFTPAREN pointF TOKEN_COMMA pointF TOKEN_RIGHTPAREN {
           $$ = new QVariant();
           *$$ = QLineF($3->toPointF(), $5->toPointF());
       }
     | TOKEN_LINEF TOKEN_LEFTPAREN line TOKEN_RIGHTPAREN {
           $$ = new QVariant();
           *$$ = QLineF($3->toLine());
       }
     | TOKEN_LINEF TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_COMMA number TOKEN_COMMA number TOKEN_RIGHTPAREN {
           $$ = new QVariant();
           *$$ = QLineF($3->toFloat(), $5->toFloat(), $7->toFloat(), $9->toFloat());
       }
     ;

point: TOKEN_POINT TOKEN_LEFTPAREN TOKEN_RIGHTPAREN {$$ = new QVariant(QPoint());}
     | TOKEN_POINT TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_RIGHTPAREN {
           $$ = new QVariant();
           *$$ = QPoint($3->toFloat(), $5->toFloat());
       }
     ;

pointF: TOKEN_POINTF TOKEN_LEFTPAREN TOKEN_RIGHTPAREN {$$ = new QVariant(QPointF());}
      | TOKEN_POINTF TOKEN_LEFTPAREN point TOKEN_RIGHTPAREN {
            $$ = new QVariant();
            *$$ = QPointF($3->toPoint());
        }
      | TOKEN_POINTF TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_RIGHTPAREN {
            $$ = new QVariant();
            *$$ = QPointF($3->toFloat(), $5->toFloat());
        }
      ;

size: TOKEN_SIZE TOKEN_LEFTPAREN TOKEN_RIGHTPAREN {$$ = new QVariant(QSize());}
    | TOKEN_SIZE TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_RIGHTPAREN {
          $$ = new QVariant();
          *$$ = QSize($3->toFloat(), $5->toFloat());
      }
    ;

sizeF: TOKEN_SIZEF TOKEN_LEFTPAREN TOKEN_RIGHTPAREN {$$ = new QVariant(QSizeF());}
     | TOKEN_SIZEF TOKEN_LEFTPAREN size TOKEN_RIGHTPAREN {
           $$ = new QVariant();
           *$$ = QSizeF($3->toSize());
       }
     | TOKEN_SIZEF TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_RIGHTPAREN {
           $$ = new QVariant();
           *$$ = QSizeF($3->toFloat(), $5->toFloat());
       }
     ;

dateTime: TOKEN_DATETIME TOKEN_LEFTPAREN TOKEN_RIGHTPAREN {$$ = new QVariant(QDateTime());}
        | TOKEN_DATETIME TOKEN_LEFTPAREN date TOKEN_RIGHTPAREN {
              $$ = new QVariant();
              *$$ = QDateTime($3->toDate());
          }
        | TOKEN_DATETIME TOKEN_LEFTPAREN date TOKEN_COMMA time TOKEN_RIGHTPAREN {
              $$ = new QVariant();
              *$$ = QDateTime($3->toDate(), $5->toTime());
          }
        ;

date: TOKEN_DATE TOKEN_LEFTPAREN TOKEN_RIGHTPAREN {$$ = new QVariant(QDate());}
    | TOKEN_DATE TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_COMMA number TOKEN_RIGHTPAREN {
          $$ = new QVariant();
          *$$ = QDate($3->toFloat(), $5->toFloat(), $7->toFloat());
      }
    ;

time: TOKEN_TIME TOKEN_LEFTPAREN TOKEN_RIGHTPAREN {$$ = new QVariant(QTime());}
    | TOKEN_TIME TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_RIGHTPAREN {
          $$ = new QVariant();
          *$$ = QTime($3->toFloat(), $5->toFloat());
      }
    | TOKEN_TIME TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_COMMA number TOKEN_RIGHTPAREN {
          $$ = new QVariant();
          *$$ = QTime($3->toFloat(), $5->toFloat(), $7->toFloat());
      }
    | TOKEN_TIME TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_COMMA number TOKEN_COMMA number TOKEN_RIGHTPAREN {
          $$ = new QVariant();
          *$$ = QTime($3->toFloat(), $5->toFloat(), $7->toFloat(), $9->toFloat());
      }
    ;

color: TOKEN_COLOR TOKEN_LEFTPAREN TOKEN_RIGHTPAREN {$$ = new QVariant(QColor());}
     | TOKEN_COLOR TOKEN_LEFTPAREN TOKEN_STRING TOKEN_RIGHTPAREN {
           $$ = new QVariant();
           *$$ = QColor($3->toString());
       }
     | TOKEN_COLOR TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_COMMA number TOKEN_RIGHTPAREN {
           $$ = new QVariant();
           *$$ = QColor($3->toFloat(), $5->toFloat(), $7->toFloat());
       }
     | TOKEN_COLOR TOKEN_LEFTPAREN number TOKEN_COMMA number TOKEN_COMMA number TOKEN_COMMA number TOKEN_RIGHTPAREN {
           $$ = new QVariant();
           *$$ = QColor($3->toFloat(), $5->toFloat(), $7->toFloat(), $9->toFloat());
       }
     ;

bytes: TOKEN_BYTES TOKEN_STRING  {
           $$ = new QVariant();
           *$$ = $2->toString().toUtf8();
       }
     ;

url: TOKEN_URL TOKEN_LEFTPAREN TOKEN_RIGHTPAREN {$$ = new QVariant(QUrl());}
   | TOKEN_URL TOKEN_LEFTPAREN TOKEN_STRING TOKEN_RIGHTPAREN {
         $$ = new QVariant();
         *$$ = QUrl($3->toString());
     }
   ;

variantList: TOKEN_LEFTBRACKET TOKEN_RIGHTBRACKET {$$ = new QVariant(QVariantList());}
           | TOKEN_LEFTBRACKET variantListItems TOKEN_RIGHTBRACKET {
                 $$ = new QVariant();
                 *$$ = $2->toList();
             }
           ;

variantListItems: variant {
                      $$ = new QVariant();

                      QVariantList variantList;

                      variantList << *$1;

                      *$$ = variantList;
                  }
                | variantListItems TOKEN_COMMA variant {
                      $$ = new QVariant();

                      QVariantList variantList($1->toList());

                      variantList << *$3;

                      *$$ = variantList;
                  }
                ;

variantMap: TOKEN_LEFTCURLYBRACKET TOKEN_RIGHTCURLYBRACKET {$$ = new QVariant(QVariantMap());}
          | TOKEN_LEFTCURLYBRACKET variantMapItems TOKEN_RIGHTCURLYBRACKET {
                $$ = new QVariant();
                *$$ = $2->toMap();
            }
          ;

variantMapItems: variantMapPair {
                     $$ = new QVariant();

                     QVariantMap variantMap;
                     QVariantList pair = $1->toList();

                     variantMap[pair[0].toString()] = pair[1];

                     *$$ = variantMap;
                 }
               | variantMapItems TOKEN_COMMA variantMapPair {
                     $$ = new QVariant();

                     QVariantMap variantMap($1->toMap());
                     QVariantList pair = $3->toList();

                     variantMap[pair[0].toString()] = pair[1];

                     *$$ = variantMap;
                 }
               ;

variantMapPair: TOKEN_STRING TOKEN_COLON variant {
                    $$ = new QVariant();

                    QVariantList variantList;

                    variantList << $1->toString() << *$3;

                    *$$ = variantList;
                }
              ;

number: TOKEN_INTIGER
      | TOKEN_FLOAT
      ;

%%

void yyerror(const char *s)
{
    qDebug() << "error:" << s;
}
