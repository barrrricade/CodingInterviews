class User < ApplicationRecord
  # Include default devise modules. Others available are:
  # :confirmable, :lockable, :timeoutable, :trackable and :omniauthable
  devise :database_authenticatable, :registerable,
         :recoverable, :rememberable, :validatable

  has_many :articles, dependent: :destroy
  validates :name, length: {minimum: 3}, presence: true

  include PgSearch::Model
  multisearchable against: :name,
    using: { tsearch: { prefix: true } }


  # include PgSearch::Model
  # pg_search_scope :search_users,
  #   against: [:name],
  #   using: {
  #     tsearch: { prefix: true } # <-- now `superman batm` will return something!
  #   }
end
